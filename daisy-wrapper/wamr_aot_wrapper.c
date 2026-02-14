#include "wamr_aot_wrapper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// Embedded AOT Module
#include "../wasm-module/build/module_aot.h"

#define STACK_SIZE 8192
#define HEAP_SIZE (16 * 1024)  // App heap for WASM malloc - FAUST modules use static arrays

// Forward declarations for SDRAM allocator functions
extern void* sdram_alloc(size_t size);
extern void* sdram_realloc(void* ptr, size_t size);
extern void sdram_dealloc(void* ptr);
extern void* sdram_calloc(size_t nmemb, size_t size);

// Native math wrappers for WAMR (first param is always wasm_exec_env_t)
static float native_tanhf(wasm_exec_env_t env, float x) { return tanhf(x); }
static float native_expf(wasm_exec_env_t env, float x) { return expf(x); }
static float native_logf(wasm_exec_env_t env, float x) { return logf(x); }
static float native_sinf(wasm_exec_env_t env, float x) { return sinf(x); }
static float native_cosf(wasm_exec_env_t env, float x) { return cosf(x); }
static float native_tanf(wasm_exec_env_t env, float x) { return tanf(x); }
static double native_tanh(wasm_exec_env_t env, double x) { return tanh(x); }
static double native_exp(wasm_exec_env_t env, double x) { return exp(x); }
static double native_log(wasm_exec_env_t env, double x) { return log(x); }
static double native_sin(wasm_exec_env_t env, double x) { return sin(x); }
static double native_cos(wasm_exec_env_t env, double x) { return cos(x); }
static double native_tan(wasm_exec_env_t env, double x) { return tan(x); }

static NativeSymbol native_math_symbols[] = {
    { "tanhf", (void*)native_tanhf, "(f)f", NULL },
    { "expf",  (void*)native_expf,  "(f)f", NULL },
    { "logf",  (void*)native_logf,  "(f)f", NULL },
    { "sinf",  (void*)native_sinf,  "(f)f", NULL },
    { "cosf",  (void*)native_cosf,  "(f)f", NULL },
    { "tanf",  (void*)native_tanf,  "(f)f", NULL },
    { "tanh",  (void*)native_tanh,  "(F)F", NULL },
    { "exp",   (void*)native_exp,   "(F)F", NULL },
    { "log",   (void*)native_log,   "(F)F", NULL },
    { "sin",   (void*)native_sin,   "(F)F", NULL },
    { "cos",   (void*)native_cos,   "(F)F", NULL },
    { "tan",   (void*)native_tan,   "(F)F", NULL },
};

#define NUM_NATIVE_MATH_SYMBOLS (sizeof(native_math_symbols) / sizeof(NativeSymbol))

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

    // Register native math functions so WASM imports resolve to ARM-native code
    if (!wasm_runtime_register_natives("env", native_math_symbols, NUM_NATIVE_MATH_SYMBOLS)) {
        wamr_print("ERROR: Failed to register native math functions\n");
        sdram_dealloc(engine);
        return NULL;
    }
    wamr_print("Registered %d native math functions\n", (int)NUM_NATIVE_MATH_SYMBOLS);

    return engine;
}

void wamr_aot_engine_delete(WamrAotEngine* engine) {
    if (!engine) return;
    if (engine->instance) {
        if (engine->input_offset) wasm_runtime_module_free(engine->instance, engine->input_offset);
        if (engine->output_offset) wasm_runtime_module_free(engine->instance, engine->output_offset);
    }
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

    // Pre-allocate WASM-side I/O buffers (128 samples = typical audio block size)
    engine->buffer_samples = 128;
    engine->input_offset = wasm_runtime_module_malloc(engine->instance,
        engine->buffer_samples * sizeof(float), NULL);
    engine->output_offset = wasm_runtime_module_malloc(engine->instance,
        engine->buffer_samples * sizeof(float), NULL);

    if (engine->input_offset == 0 || engine->output_offset == 0) {
        wamr_print("ERROR: Failed to pre-allocate WASM I/O buffers\n");
        if (engine->input_offset) wasm_runtime_module_free(engine->instance, engine->input_offset);
        if (engine->output_offset) wasm_runtime_module_free(engine->instance, engine->output_offset);
        engine->input_offset = 0;
        engine->output_offset = 0;
        engine->buffer_samples = 0;
        return false;
    }
    wamr_print("Pre-allocated WASM I/O buffers for %d samples\n", engine->buffer_samples);

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

    if (num_samples > engine->buffer_samples || engine->input_offset == 0) {
        wamr_print("ERROR: num_samples %d exceeds pre-allocated buffer %d\n",
                    num_samples, engine->buffer_samples);
        return;
    }

    // Copy input into pre-allocated WASM buffer
    void* wasm_input_ptr = wasm_runtime_addr_app_to_native(engine->instance, engine->input_offset);
    if (wasm_input_ptr) {
        memcpy(wasm_input_ptr, input, num_samples * sizeof(float));
    }

    // Call the process function with (input_ptr, output_ptr, num_samples)
    uint32_t argv[3];
    argv[0] = engine->input_offset;
    argv[1] = engine->output_offset;
    argv[2] = num_samples;

    if (wasm_runtime_call_wasm(engine->exec_env, engine->process_func, 3, argv)) {
        // Copy output from pre-allocated WASM buffer
        void* wasm_output_ptr = wasm_runtime_addr_app_to_native(engine->instance, engine->output_offset);
        if (wasm_output_ptr) {
            memcpy(output, wasm_output_ptr, num_samples * sizeof(float));
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
}
