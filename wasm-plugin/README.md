# WASM Plugin Build Instructions

This directory contains the oscillator plugin that will be compiled to WASM and then to AOT for the Daisy Seed.

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
1. Compile `oscillator.c` to `oscillator.wasm`
2. Compile `oscillator.wasm` to `oscillator.aot` (native ARM code for Cortex-M7)
3. Generate `oscillator_aot.h` with embedded binary data

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
    -o oscillator.wasm \
    oscillator.c

# Step 2: WASM to AOT
../wasm-micro-runtime/wamr-compiler/build/wamrc \
    --target=thumbv7em \
    --cpu=cortex-m7 \
    --size-level=3 \
    --enable-builtin-intrinsics=i64.common,fp.common \
    -o oscillator.aot \
    oscillator.wasm

# Step 3: Embed in header
xxd -i oscillator.aot > oscillator_aot.h
```
