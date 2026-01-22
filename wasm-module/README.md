# WASM Module Build Instructions

This directory contains the module that will be compiled to WASM and then to AOT for the Daisy Seed.

## Prerequisites

1. **WASI SDK** - Download from https://github.com/WebAssembly/wasi-sdk/releases
   - Extract to `/opt/wasi-sdk` or set `WASI_SDK` environment variable

2. **wamrc compiler** - Built from wasm-micro-runtime
   - The build script will attempt to build it if not found

## Building

```bash
./build.sh
```

This will:
1. Compile `module.cpp` to `module.wasm`
2. Compile `module.wasm` to `module.aot` (native ARM code for Cortex-M7)
3. Generate `module_aot.h` with embedded binary data

## Manual Build Steps

If you need to build manually:

```bash
# Step 1: C to WASM
$WASI_SDK/bin/clang \
    --target=wasm32-wasi \
    -O3 \
    -nostdlib \
    -Wl,--no-entry \
    -Wl,--export-all \
    -Wl,--allow-undefined \
    -o module.wasm \
    module.cpp

# Step 2: WASM to AOT
../wasm-micro-runtime/wamr-compiler/build/wamrc \
    --target=thumbv7em \
    --cpu=cortex-m7 \
    --size-level=3 \
    --enable-builtin-intrinsics=i64.common,fp.common \
    -o module.aot \
    module.wasm

# Step 3: Embed in header
xxd -i module.aot > module_aot.h
```
