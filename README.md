# WAMR AOT Demo for Daisy Seed

This project demonstrates embedding WAMR (WebAssembly Micro Runtime) with AOT compilation into a Daisy Seed application.

## Quick Start

### 1. Setup Prerequisites

First, install WASI SDK (needed to compile C to WASM):

```bash
./setup-wasi-sdk.sh
```

This will download and install WASI SDK to `/opt/wasi-sdk`.

### 2. Build WASM Plugin

```bash
cd wasm-plugin
./build.sh
cd ..
```

This compiles the oscillator plugin:
- C source → WASM bytecode
- WASM → AOT (native ARM code for Cortex-M7)
- AOT → Embedded C header

The first build will also compile `wamrc` (the AOT compiler), which takes several minutes.

### 3. Build and Flash Daisy Application

```bash
make
# or
./run.sh
```

## Project Structure

```
wamr-demo/
├── src/
│   └── main.cpp              # Main application with WAMR integration
├── wasm-plugin/
│   ├── oscillator.c          # Plugin source code
│   ├── oscillator.wasm       # Compiled WASM bytecode
│   ├── oscillator.aot        # AOT-compiled ARM code
│   ├── oscillator_aot.h      # Embedded binary (auto-generated)
│   └── build.sh              # Plugin build script
├── wasm-micro-runtime/       # WAMR submodule
├── wamr.mk                   # WAMR build configuration
└── Makefile                  # Main build system
```

## What It Does

1. Initializes WAMR runtime (AOT-only mode, ~29KB footprint)
2. Loads embedded oscillator AOT module
3. Calls WASM functions to generate sine wave samples
4. Runs performance benchmarks (48,000 iterations)
5. Analyzes real-time capability for audio processing

## Expected Output

Connect via USB serial to see:
- Runtime initialization
- AOT module loading
- Function execution tests
- Benchmark results with timing metrics
- Real-time performance analysis

## WAMR Configuration

This build uses:
- **AOT-only mode** (no interpreter)
- **128KB memory pool** for WAMR
- **8KB stack + 16KB heap** per WASM instance
- **Built-in libc** (minimal, no WASI)
- **Cortex-M7 optimization** with FPU intrinsics

## Modifying the Plugin

Edit `wasm-plugin/oscillator.c` and rebuild:

```bash
cd wasm-plugin
./build.sh
cd ..
make clean && make
```

The AOT module will be regenerated and embedded automatically.

## Performance Notes

- AOT mode achieves 80-95% of native C performance
- Single sample generation: typically < 1 microsecond
- Suitable for real-time audio processing at 48kHz
- Memory footprint: ~50KB total (runtime + instance)

## Troubleshooting

**"WASI SDK not found"**
- Run `./setup-wasi-sdk.sh`
- Or set `WASI_SDK` environment variable

**"wamrc not found"**
- First plugin build will compile it automatically
- Takes 5-10 minutes on first run

**Build errors**
- Ensure libDaisy submodule is initialized: `./init.sh`
- Check ARM toolchain is installed: `arm-none-eabi-gcc --version`

## Next Steps

Try these modifications:
1. Add more audio effects (delay, filter, distortion)
2. Load multiple plugins simultaneously
3. Store plugins in QSPI flash for dynamic loading
4. Add parameter controls from Daisy hardware

---

## Original README

# daisy-kickstart

Quickly get up && running with Daisy 

## Getting Started

1. First, [install the Daisy Toolchain](https://daisy.audio/tutorials/cpp-dev-env/#1-install-the-toolchain). 
2. Once installed, run the `init.sh` script to configure your local copy of this repository. 
3. This repository is configured for building SRAM apps. Connect your Daisy Seed via USB and [install a bootloader](https://flash.daisy.audio/) before proceeding.
4. With your device in program mode, use `run.sh` (or `SHIFT+CMD+B` in VSCode) to build programs and flash them to your Daisy.

> [!NOTE]
> When developing for the Daisy, it is often useful to use serial monitoring for testing and debugging. Many examples in `examples/` demonstrate this. If developing in VSCode, we recommend installing Microsoft's [serial monitor extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-serial-monitor), which will add easy access to serial monitoring via the terminal panel.