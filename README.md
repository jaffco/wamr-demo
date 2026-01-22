# WAMR AOT Demo for Daisy Seed

This project demonstrates embedding WAMR (WebAssembly Micro Runtime) with AOT compilation into a Daisy Seed application.

## Quick Start

### 1. Setup Prerequisites

First, install WASI SDK (needed to compile C to WASM):

```bash
./setup-wasi-sdk.sh
```

This will download and install WASI SDK to `/opt/wasi-sdk`.

### 2. Build WASM Module

```bash
cd wasm-module
./build-wasm.sh
cd ..
```

This compiles the module:
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
├── wasm-module/
│   ├── build/
│   │   ├── module.wasm       # Compiled WASM bytecode
│   │   ├── module.aot        # AOT-compiled ARM code
│   │   └── module_aot.h      # Embedded binary (auto-generated)
│   ├── module.cpp            # Module source code
│   └── build-wasm.sh              # Module build script
├── wasm-micro-runtime/       # WAMR submodule
├── wamr.mk                   # WAMR build configuration
└── Makefile                  # Main build system
```

## What It Does

1. Initializes WAMR runtime (AOT-only mode, ~29KB footprint)
2. Loads embedded module AOT
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

## Modifying the Module

Edit `wasm-module/module.cpp` and rebuild:

```bash
cd wasm-module
./build-wasm.sh
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
- First module build will compile it automatically
- Takes 5-10 minutes on first run

**Build errors**
- Ensure libDaisy submodule is initialized: `./init.sh`
- Check ARM toolchain is installed: `arm-none-eabi-gcc --version`

## Next Steps

Try these modifications:
1. Add more audio effects (delay, filter, distortion)
2. Load multiple modules simultaneously
3. Store modules in QSPI flash for dynamic loading
4. Add parameter controls from Daisy hardware