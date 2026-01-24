#include "../libDaisy/src/daisy_seed.h"
#include "SDRAM.hpp"

// WAMR Runtime Headers
extern "C" {
#include "wasm_export.h"
}

// Daisy WAMR Wrapper
#include "../daisy-wrapper/wamr_aot_wrapper.h"

using namespace daisy;
using namespace Jaffx;

// Global SDRAM allocator instance
static SDRAM sdram;

// Macro for halting on errors
#define ERROR_HALT while (true) {}

// Macro for enabling audio
#define RUN_AUDIO

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

// WAMR Engine using Daisy wrapper
static WamrAotEngine* wamr_engine = nullptr;

// Forward declarations for WAMR platform allocator functions
extern "C" {
void *os_malloc(unsigned size);
void *os_realloc(void *ptr, unsigned size);
void os_free(void *ptr);
}

/**
 * Initialize WAMR runtime using Daisy wrapper
 */
bool InitWAMR() {
    hardware.PrintLine("Initializing WAMR runtime with Daisy wrapper...");

    // Create WAMR engine (uses SDRAM allocator internally)
    wamr_engine = wamr_aot_engine_new();
    if (!wamr_engine) {
        hardware.PrintLine("ERROR: Failed to create WAMR engine");
        ERROR_HALT
    }

    hardware.PrintLine("WAMR engine created (using SDRAM allocator)");

    // Load embedded AOT module
    hardware.PrintLine("Loading embedded AOT module...");

    if (!wamr_aot_engine_load_embedded_module(wamr_engine)) {
        hardware.PrintLine("ERROR: Failed to load embedded AOT module");
        wamr_aot_engine_delete(wamr_engine);
        wamr_engine = nullptr;
        ERROR_HALT
    }

    hardware.PrintLine("Embedded AOT module loaded and instantiated");
    hardware.PrintLine("Function resolved: process(float*, float*, int)");

    return true;
}

/**
 * Call WASM function: process(float* input, float* output, int num_samples)
 * Wrapper for single sample processing using buffer-based interface
 */
float CallProcess(float input) {
    float input_buffer[1] = {input};
    float output_buffer[1] = {0.0f};

    wamr_aot_engine_process(wamr_engine, input_buffer, output_buffer, 1);

    return output_buffer[0];
}

// Audio callback using buffer-based WAMR processing
static void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
    // Process the entire buffer at once using the WAMR wrapper
    wamr_aot_engine_process(wamr_engine, in[0], out[0], size);

    // Copy left channel to right channel for stereo output
    memcpy(out[1], out[0], size * sizeof(float));
}

int main() {
    hardware.Init();
    hardware.StartLog(true);

    System::Delay(200);
    hardware.PrintLine("===========================================");
    hardware.PrintLine("    WAMR AOT Demo - Daisy Wrapper     ");
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
    System::Delay(200);

    #ifndef RUN_AUDIO
    // Prepare for next test
    System::ResetToBootloader(System::BootloaderMode::DAISY_INFINITE_TIMEOUT);
    #endif

    // Start Audio
    hardware.SetAudioBlockSize(128); // number of samples handled per callback (buffer size)
	hardware.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ); // sample rate
    hardware.StartAudio(AudioCallback);
    return 0;
}