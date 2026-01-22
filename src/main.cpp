#include "../libDaisy/src/daisy_seed.h"
#include "SDRAM.hpp"

// WAMR Runtime Headers
extern "C" {
#include "wasm_export.h"
}

// Embedded AOT Module
#include "../wasm-module/module_aot.h"

using namespace daisy;
using namespace Jaffx;

// Global SDRAM allocator instance
static SDRAM sdram;

// Macro for halting on errors
#define ERROR_HALT while (true) {}

// C wrapper functions for WAMR platform to use SDRAM
extern "C" {
    void* sdram_alloc(size_t size) {
        return sdram.malloc(size);
    }
    
    void sdram_dealloc(void* ptr) {
        if (ptr) sdram.free(ptr);
    }
    
    void* sdram_realloc(void* ptr, size_t size) {
        return sdram.realloc(ptr, size);
    }
    
    void* sdram_calloc(size_t nmemb, size_t size) {
        return sdram.calloc(nmemb, size);
    }
}

class JaffxTimer {
private:
  bool mDone = false;
  unsigned int mStartTime = 0;
  unsigned int mEndTime = 0;
  unsigned int mTickFreq = 0; 

public:
  void start() {  
    mTickFreq = System::GetTickFreq();
    mStartTime = System::GetTick();
  }

  void end() {
    mEndTime = System::GetTick();
    mDone = true;
  }

  unsigned int ticksElapsed() {
    if (!mDone) {
      return 0;
    }
    return mEndTime - mStartTime;
  }

  float usElapsed() {
    if (!mDone) {
      return 0.f;
    }
    float ticksElapsed = float(mEndTime - mStartTime);
    return (ticksElapsed * 1e6f) / mTickFreq;
  }
};

static DaisySeed hardware;

// WAMR Runtime Globals
static char *global_heap_buf = nullptr;  // Will be allocated from SDRAM
static const size_t WAMR_HEAP_SIZE = 256 * 1024; // 256KB for WAMR (using SDRAM)
static wasm_module_t wasm_module = nullptr;
static wasm_module_inst_t wasm_module_inst = nullptr;
static wasm_exec_env_t exec_env = nullptr;

// Function reference - single exported function
static wasm_function_inst_t func_process = nullptr;

/**
 * Initialize WAMR runtime and load embedded AOT module
 */
bool InitWAMR() {
    char error_buf[128];
    RuntimeInitArgs init_args;
    
    hardware.PrintLine("Initializing WAMR runtime...");
    // Allocate heap buffer from SDRAM
    global_heap_buf = (char *)sdram.malloc(WAMR_HEAP_SIZE);
    if (!global_heap_buf) {
        hardware.PrintLine("ERROR: Failed to allocate WAMR heap from SDRAM");
        ERROR_HALT
    }
    hardware.PrintLine("Allocated %u KB from SDRAM for WAMR", WAMR_HEAP_SIZE / 1024);
    
    // Configure WAMR with fixed memory pool
    memset(&init_args, 0, sizeof(RuntimeInitArgs));
    init_args.mem_alloc_type = Alloc_With_Pool;
    init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
    init_args.mem_alloc_option.pool.heap_size = WAMR_HEAP_SIZE;
    
    if (!wasm_runtime_full_init(&init_args)) {
        hardware.PrintLine("ERROR: Failed to initialize WAMR runtime");
        ERROR_HALT
    }
    
    hardware.PrintLine("WAMR runtime initialized (AOT-only mode)");
    hardware.PrintLine("Heap pool: %u KB", WAMR_HEAP_SIZE / 1024);
    
    // Load embedded AOT module
    hardware.PrintLine("Loading embedded AOT module...");
    hardware.PrintLine("AOT size: %u bytes", module_aot_len);
    
    wasm_module = wasm_runtime_load(module_aot, module_aot_len,
                                     error_buf, sizeof(error_buf));
    if (!wasm_module) {
        hardware.PrintLine("ERROR: Failed to load AOT module: %s", error_buf);
        wasm_runtime_destroy();
        ERROR_HALT
    }
    
    hardware.PrintLine("AOT module loaded successfully");
    
    // Instantiate module with stack and heap
    uint32_t stack_size = 8 * 1024;    // 8KB stack
    uint32_t heap_size = 0;             // Try with no WASM heap first
    
    hardware.PrintLine("Instantiating module (stack: %uKB, heap: %uKB)...", 
                      stack_size / 1024, heap_size / 1024);
    
    wasm_module_inst = wasm_runtime_instantiate(wasm_module, stack_size, heap_size,
                                                 error_buf, sizeof(error_buf));
    if (!wasm_module_inst) {
        hardware.PrintLine("ERROR: Failed to instantiate module: %s", error_buf);
        wasm_runtime_unload(wasm_module);
        wasm_runtime_destroy();
        ERROR_HALT
    }
    
    hardware.PrintLine("Module instantiated (stack: %uKB, heap: %uKB)", 
                      stack_size / 1024, heap_size / 1024);
    
    // Create execution environment
    exec_env = wasm_runtime_create_exec_env(wasm_module_inst, stack_size);
    if (!exec_env) {
        hardware.PrintLine("ERROR: Failed to create execution environment");
        wasm_runtime_deinstantiate(wasm_module_inst);
        wasm_runtime_unload(wasm_module);
        wasm_runtime_destroy();
        ERROR_HALT
    }
    
    hardware.PrintLine("Looking up exported function 'process'");
    func_process = wasm_runtime_lookup_function(wasm_module_inst, "process");
    
    if (!func_process) {
        hardware.PrintLine("ERROR: Could not find process function");
        ERROR_HALT
    }
    
    hardware.PrintLine("Function resolved: process(float) -> float");
    
    return true;
}

/**
 * Call WASM function: process(float input) -> float
 */
float CallProcess(float input) {
    uint32_t argv[1];
    
    // Convert float input to bit pattern in argv
    memcpy(&argv[0], &input, sizeof(float));
    
    // hardware.PrintLine("  [DEBUG] Calling WASM with input=" FLT_FMT3, FLT_VAR3(input));
    
    if (!wasm_runtime_call_wasm(exec_env, func_process, 1, argv)) {
        hardware.PrintLine("ERROR: Failed to call process function");
        ERROR_HALT
    }
    
    // hardware.PrintLine("  [DEBUG] WASM call returned");
    
    // Return value is in argv[0] as bit pattern
    float result;
    memcpy(&result, &argv[0], sizeof(float));
    return result;
}

int main() {
    hardware.Init();
    hardware.StartLog(true);

    System::Delay(200);
    hardware.PrintLine("===========================================");
    hardware.PrintLine("    WAMR AOT Demo - Module     ");
    hardware.PrintLine("===========================================");
    hardware.PrintLine("");
    
    // Initialize SDRAM allocator
    hardware.PrintLine("Initializing SDRAM allocator...");
    sdram.init();
    hardware.PrintLine("SDRAM initialized (64MB at 0xC0000000)");
    hardware.PrintLine("");
    
    // Initialize WAMR and load AOT module
    if (!InitWAMR()) {
        hardware.PrintLine("FATAL: WAMR initialization failed");
        ERROR_HALT
    }
    
    hardware.PrintLine("=== Testing Process Function ===");
    
    // Generate a few test samples with different inputs
    hardware.PrintLine("Calling process() with various inputs...");
    for (int i = 0; i < 5; i++) {
        float input = (float)i * 0.1f;  // 0.0, 0.1, 0.2, 0.3, 0.4
        float output = CallProcess(input);
        hardware.PrintLine("  process(" FLT_FMT3 ") = " FLT_FMT3, FLT_VAR3(input), FLT_VAR3(output));
    }
    
    hardware.PrintLine("");
    hardware.PrintLine("=== Running Performance Benchmarks ===");
    System::Delay(100);
    
    // Benchmark configuration (same as your example)
    const int WARMUP_RUNS = 10;
    const int BENCHMARK_RUNS = 48000;  // 1 second at 48kHz
    
    // Warmup phase
    hardware.PrintLine("");
    hardware.PrintLine("[WARMUP] Running %d warmup iterations...", WARMUP_RUNS);
    volatile float warmup_result = 0.0f;
    for (int i = 0; i < WARMUP_RUNS; i++) {
        float input = (i % 10) / 10.0f;
        warmup_result += CallProcess(input);
    }
    hardware.PrintLine("[OK] Warmup complete (result=" FLT_FMT3 ")", FLT_VAR3(warmup_result));
    
    // Benchmark phase
    hardware.PrintLine("");
    hardware.PrintLine("[BENCHMARK] Running %d iterations...", BENCHMARK_RUNS);
    
    float total_us = 0.0f;
    float min_us = 1e9f;
    float max_us = 0.0f;
    float total_ticks = 0.0f;
    float min_ticks = 1e9f;
    float max_ticks = 0.0f;
    volatile float checksum = 0.0f;
    
    for (int i = 0; i < BENCHMARK_RUNS; i++) {
        // Use varying input to prevent optimization
        float input = (i % 100) / 100.0f;
        
        JaffxTimer timer;
        timer.start();
        float result = CallProcess(input);
        timer.end();
        
        float elapsed_us = timer.usElapsed();
        float elapsed_ticks = (float)timer.ticksElapsed();
        
        total_us += elapsed_us;
        total_ticks += elapsed_ticks;
        
        if (elapsed_us < min_us) min_us = elapsed_us;
        if (elapsed_us > max_us) max_us = elapsed_us;
        if (elapsed_ticks < min_ticks) min_ticks = elapsed_ticks;
        if (elapsed_ticks > max_ticks) max_ticks = elapsed_ticks;
        
        checksum += result;
    }
    
    float avg_us = total_us / BENCHMARK_RUNS;
    float avg_ticks = total_ticks / BENCHMARK_RUNS;
    
    hardware.PrintLine("");
    hardware.PrintLine("=== BENCHMARK RESULTS ===");
    hardware.PrintLine("Iterations: %d", BENCHMARK_RUNS);
    hardware.PrintLine("Average:    " FLT_FMT3 " us (%d ticks)", FLT_VAR3(avg_us), (int)avg_ticks);
    hardware.PrintLine("Minimum:    " FLT_FMT3 " us (%d ticks)", FLT_VAR3(min_us), (int)min_ticks);
    hardware.PrintLine("Maximum:    " FLT_FMT3 " us (%d ticks)", FLT_VAR3(max_us), (int)max_ticks);
    hardware.PrintLine("Checksum:   " FLT_FMT3 " (prevents optimization)", FLT_VAR3(checksum));
    
    // Calculate real-time performance
    float samples_per_us = 1.0f / avg_us;
    float samples_per_sec = samples_per_us * 1000000.0f;
    float realtime_factor_48k = samples_per_sec / 48000.0f;
    
    hardware.PrintLine("");
    hardware.PrintLine("=== REAL-TIME ANALYSIS ===");
    hardware.PrintLine("Sample rate: 48000 Hz");
    hardware.PrintLine("Throughput: " FLT_FMT3 " samples/sec", FLT_VAR3(samples_per_sec));
    hardware.PrintLine("Real-time factor: " FLT_FMT3 "x", FLT_VAR3(realtime_factor_48k));
    
    if (realtime_factor_48k >= 1.0f) {
        hardware.PrintLine("Result: CAN run in REAL-TIME! OK");
    } else {
        hardware.PrintLine("Result: Too slow for real-time X");
    }
    
    hardware.PrintLine("");
    hardware.PrintLine("[SUCCESS] WAMR AOT benchmark complete!");

    hardware.PrintLine("Test Complete!");
    hardware.SetLed(true);

    // Prepare for next test
    System::Delay(200);
    System::ResetToBootloader(System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
    return 0;
}