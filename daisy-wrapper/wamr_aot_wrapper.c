#include "wamr_aot_wrapper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Embedded AOT Module
#include "../wasm-module/build/module_aot.h"

#define STACK_SIZE 8192
#define HEAP_SIZE (16 * 1024)  // App heap for WASM malloc - FAUST modules use static arrays

// Forward declarations for SDRAM allocator functions
extern void* sdram_alloc(size_t size);
extern void* sdram_realloc(void* ptr, size_t size);
extern void sdram_dealloc(void* ptr);
extern void* sdram_calloc(size_t nmemb, size_t size);

// Global print callback
wamr_print_callback_t wamr_print_callback = NULL;

// Print function using callback
void wamr_print(const char* format, ...) {
    if (!wamr_print_callback) return;
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    wamr_print_callback(buffer);
}

// Wrapper to use calloc instead of malloc for zero-initialization
static void* wamr_calloc_wrapper(unsigned size) {
    // Use calloc(1, size) to get zero-initialized memory
    return sdram_calloc(1, size);
}

WamrAotEngine* wamr_aot_engine_new(void) {
    WamrAotEngine* engine = sdram_calloc(1, sizeof(WamrAotEngine));
    if (!engine) {
        wamr_print("ERROR: Failed to allocate WAMR engine\n");
        return NULL;
    }

    RuntimeInitArgs init_args = {0};
    init_args.mem_alloc_type = Alloc_With_Allocator;
    // Use calloc wrapper to ensure all WAMR allocations are zero-initialized
    init_args.mem_alloc_option.allocator.malloc_func = (void*)wamr_calloc_wrapper;
    init_args.mem_alloc_option.allocator.realloc_func = (void*)sdram_realloc;
    init_args.mem_alloc_option.allocator.free_func = (void*)sdram_dealloc;

    wamr_print("Initializing WAMR runtime...\n");
    if (!wasm_runtime_full_init(&init_args)) {
        wamr_print("ERROR: wasm_runtime_full_init failed\n");
        sdram_dealloc(engine);
        return NULL;
    }
    wamr_print("WAMR runtime initialized successfully\n");

    return engine;
}

void wamr_aot_engine_delete(WamrAotEngine* engine) {
    if (!engine) return;
    if (engine->exec_env) wasm_runtime_destroy_exec_env(engine->exec_env);
    if (engine->instance) wasm_runtime_deinstantiate(engine->instance);
    if (engine->module) wasm_runtime_unload(engine->module);
    wasm_runtime_destroy();
    sdram_dealloc(engine);
}

bool wamr_aot_engine_load_embedded_module(WamrAotEngine* engine) {
    char error_buf[128];

    wamr_print("Loading AOT module: %p, size: %u bytes\n", module_aot, module_aot_len);
    wamr_print("Module magic bytes: %02x %02x %02x %02x\n",
           module_aot[0], module_aot[1], module_aot[2], module_aot[3]);

    engine->module = wasm_runtime_load(module_aot, module_aot_len, error_buf, sizeof(error_buf));
    if (!engine->module) {
        wamr_print("ERROR: Failed to load embedded AOT module\n");
        wamr_print("Error buffer: '%s'\n", error_buf);
        wamr_print("Module data starts with: %02x %02x %02x %02x\n",
               module_aot[0], module_aot[1], module_aot[2], module_aot[3]);
        wamr_print("Checking SDRAM allocator...\n");

        // Test SDRAM allocator
        void* test_alloc = sdram_alloc(1024);
        if (test_alloc) {
            wamr_print("SDRAM alloc test: SUCCESS (%p)\n", test_alloc);
            sdram_dealloc(test_alloc);
        } else {
            wamr_print("SDRAM alloc test: FAILED\n");
        }

        return false;
    }

    wamr_print("AOT module loaded successfully\n");

    engine->instance = wasm_runtime_instantiate(engine->module, STACK_SIZE, HEAP_SIZE,
                                                error_buf, sizeof(error_buf));

    if (!engine->instance) {
        wamr_print("ERROR: Failed to instantiate module: %s\n", error_buf);
        return false;
    }

    engine->exec_env = wasm_runtime_create_exec_env(engine->instance, STACK_SIZE);
    if (!engine->exec_env) {
        wamr_print("ERROR: Failed to create execution environment\n");
        return false;
    }

    engine->process_func = wasm_runtime_lookup_function(engine->instance, "process");
    if (!engine->process_func) {
        wamr_print("ERROR: Could not find process function\n");
        return false;
    }

    return true;
}

void wamr_aot_engine_process(WamrAotEngine* engine, const float* input, float* output, int num_samples) {
    if (!engine->process_func) {
        wamr_print("ERROR: process_func is NULL!\n");
        return;
    }

    // Initialize WAMR thread environment for the calling thread (e.g., audio thread)
    // This is safe to call multiple times - it will return true if already initialized
    static __thread bool thread_env_initialized = false;
    if (!thread_env_initialized) {
        if (!wasm_runtime_init_thread_env()) {
            wamr_print("ERROR: Failed to initialize WAMR thread environment!\n");
            return;
        }
        thread_env_initialized = true;
        wamr_print("Initialized WAMR thread environment for audio processing thread\n");
    }

    // Get WASM module's memory instance
    uint32_t input_offset = wasm_runtime_module_malloc(engine->instance, num_samples * sizeof(float), NULL);
    uint32_t output_offset = wasm_runtime_module_malloc(engine->instance, num_samples * sizeof(float), NULL);
    
    if (input_offset == 0 || output_offset == 0) {
        wamr_print("ERROR: Failed to allocate WASM memory\n");
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
            wamr_print("WAMR process call succeeded\n");
            debug_count++;
        }
    } else {
        static int error_count = 0;
        if (error_count < 1) {
            const char* exception = wasm_runtime_get_exception(engine->instance);
            wamr_print("ERROR: WAMR call failed! Exception: %s\n", exception ? exception : "none");
            error_count++;
        }
    }
    
    // Free WASM memory
    wasm_runtime_module_free(engine->instance, input_offset);
    wasm_runtime_module_free(engine->instance, output_offset);
}
