#include "wamr_aot_wrapper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Embedded AOT Module
#include "../wasm-module/build/module_aot.h"

#define STACK_SIZE 8192
#define HEAP_SIZE (16 * 1024)  // Match main.cpp heap size

// Forward declarations for SDRAM allocator functions
// extern void* sdram_alloc(size_t size);
// extern void* sdram_realloc(void* ptr, size_t size);
// extern void sdram_dealloc(void* ptr);

// Use standard malloc for now to test
#define sdram_alloc malloc
#define sdram_realloc realloc
#define sdram_dealloc free

WamrAotEngine* wamr_aot_engine_new(void) {
    WamrAotEngine* engine = calloc(1, sizeof(WamrAotEngine));
    if (!engine) return NULL;

    RuntimeInitArgs init_args = {0};
    init_args.mem_alloc_type = Alloc_With_Allocator;
    init_args.mem_alloc_option.allocator.malloc_func = (void*)sdram_alloc;
    init_args.mem_alloc_option.allocator.realloc_func = (void*)sdram_realloc;
    init_args.mem_alloc_option.allocator.free_func = (void*)sdram_dealloc;

    if (!wasm_runtime_full_init(&init_args)) {
        free(engine);
        return NULL;
    }

    return engine;
}

void wamr_aot_engine_delete(WamrAotEngine* engine) {
    if (!engine) return;
    if (engine->exec_env) wasm_runtime_destroy_exec_env(engine->exec_env);
    if (engine->instance) wasm_runtime_deinstantiate(engine->instance);
    if (engine->module) wasm_runtime_unload(engine->module);
    wasm_runtime_destroy();
    free(engine);
}

bool wamr_aot_engine_load_embedded_module(WamrAotEngine* engine) {
    char error_buf[128];

    printf("Loading AOT module: %p, size: %u bytes\n", module_aot, module_aot_len);

    engine->module = wasm_runtime_load(module_aot, module_aot_len, error_buf, sizeof(error_buf));
    if (!engine->module) {
        printf("ERROR: Failed to load embedded AOT module\n");
        printf("Error buffer: '%s'\n", error_buf);
        printf("Module data starts with: %02x %02x %02x %02x\n",
               module_aot[0], module_aot[1], module_aot[2], module_aot[3]);
        return false;
    }

    engine->instance = wasm_runtime_instantiate(engine->module, STACK_SIZE, HEAP_SIZE,
                                                error_buf, sizeof(error_buf));

    if (!engine->instance) {
        printf("ERROR: Failed to instantiate module: %s\n", error_buf);
        return false;
    }

    engine->exec_env = wasm_runtime_create_exec_env(engine->instance, STACK_SIZE);
    if (!engine->exec_env) {
        printf("ERROR: Failed to create execution environment\n");
        return false;
    }

    engine->process_func = wasm_runtime_lookup_function(engine->instance, "process");
    if (!engine->process_func) {
        printf("ERROR: Could not find process function\n");
        return false;
    }

    return true;
}

void wamr_aot_engine_process(WamrAotEngine* engine, const float* input, float* output, int num_samples) {
    if (!engine->process_func) {
        printf("ERROR: process_func is NULL!\n");
        return;
    }

    // Initialize WAMR thread environment for the calling thread (e.g., audio thread)
    // This is safe to call multiple times - it will return true if already initialized
    static __thread bool thread_env_initialized = false;
    if (!thread_env_initialized) {
        if (!wasm_runtime_init_thread_env()) {
            printf("ERROR: Failed to initialize WAMR thread environment!\n");
            return;
        }
        thread_env_initialized = true;
        printf("Initialized WAMR thread environment for audio processing thread\n");
    }

    // Get WASM module's memory instance
    uint32_t input_offset = wasm_runtime_module_malloc(engine->instance, num_samples * sizeof(float), NULL);
    uint32_t output_offset = wasm_runtime_module_malloc(engine->instance, num_samples * sizeof(float), NULL);
    
    if (input_offset == 0 || output_offset == 0) {
        printf("ERROR: Failed to allocate WASM memory\n");
        if (input_offset) wasm_runtime_module_free(engine->instance, input_offset);
        if (output_offset) wasm_runtime_module_free(engine->instance, output_offset);
        return;
    }
    
    // Get the memory pointer and copy input buffer
    void* wasm_mem_ptr = wasm_runtime_addr_app_to_native(engine->instance, input_offset);
    if (wasm_mem_ptr) {
        memcpy(wasm_mem_ptr, input, num_samples * sizeof(float));
    }
    
    // Call the process function with (input_ptr, output_ptr, num_samples)
    uint32_t argv[3];
    argv[0] = input_offset;
    argv[1] = output_offset;
    argv[2] = num_samples;
    
    if (wasm_runtime_call_wasm(engine->exec_env, engine->process_func, 3, argv)) {
        // Copy output buffer from WASM memory
        void* output_mem_ptr = wasm_runtime_addr_app_to_native(engine->instance, output_offset);
        if (output_mem_ptr) {
            memcpy(output, output_mem_ptr, num_samples * sizeof(float));
        }
        
        static int debug_count = 0;
        if (debug_count < 3) {
            printf("WAMR process call succeeded\n");
            debug_count++;
        }
    } else {
        static int error_count = 0;
        if (error_count < 1) {
            const char* exception = wasm_runtime_get_exception(engine->instance);
            printf("ERROR: WAMR call failed! Exception: %s\n", exception ? exception : "none");
            error_count++;
        }
    }
    
    // Free WASM memory
    wasm_runtime_module_free(engine->instance, input_offset);
    wasm_runtime_module_free(engine->instance, output_offset);
}
