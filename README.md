# WAMR AOT Demo for Daisy Seed

This project demonstrates embedding WAMR (WebAssembly Micro Runtime) with AOT compilation into a Daisy Seed application.

## Quick Start

1. First, [install the Daisy Toolchain](https://daisy.audio/tutorials/cpp-dev-env/#1-install-the-toolchain) && [Emscripten](https://emscripten.org).
2. Once installed, use `./init.sh` to configure your local copy of this repository. 
3. This repository is configured for building SRAM apps. Connect your Daisy Seed via USB and [install a bootloader](https://flash.daisy.audio/) before proceeding.
4. With your device in program mode, use `./run.sh` (or `SHIFT+CMD+B` in VSCode) to build, flash, and run the program. Output will be logged to `log.txt`.

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
│   └── build-wasm.sh         # Module build script
├── wasm-micro-runtime/       # WAMR submodule
├── wamr.mk                   # WAMR build configuration
└── Makefile                  # Main build system
```

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

**"wamrc not found"**
- First module build should compile it automatically
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