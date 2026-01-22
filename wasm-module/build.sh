#!/bin/bash
set -e

WAMR_ROOT=../wasm-micro-runtime

echo "Building WASM module..."

# Check for emcc
if ! command -v emcc &> /dev/null; then
    echo "ERROR: emscripten not found!"
    echo "Please install with: brew install emscripten"
    exit 1
fi

echo "Using emscripten: $(which emcc)"

# Compile C++ to WASM using emscripten
echo "Step 1: Compiling C++ to WASM..."
emcc \
    -O3 \
    -s STANDALONE_WASM=1 \
    -s EXPORTED_FUNCTIONS='["_process"]' \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -Wl,--no-entry \
    -o module.wasm \
    module.cpp

echo "WASM module size: $(wc -c < module.wasm) bytes"

# Check for wamrc
if [ ! -f "$WAMR_ROOT/wamr-compiler/build/wamrc" ]; then
    echo ""
    echo "ERROR: wamrc not found at $WAMR_ROOT/wamr-compiler/build/wamrc"
    echo ""
    echo "Building wamrc AOT compiler (this takes a few minutes)..."
    pushd $WAMR_ROOT/wamr-compiler > /dev/null
    if [ ! -d "build" ]; then
        echo "Building LLVM dependencies..."
        ./build_llvm.sh
        mkdir -p build
        cd build
        echo "Building wamrc..."
        cmake ..
        make -j$(sysctl -n hw.ncpu)
    fi
    popd > /dev/null
    echo "wamrc build complete!"
fi

# Compile WASM to AOT for Cortex-M7
echo "Step 2: Compiling WASM to AOT..."
$WAMR_ROOT/wamr-compiler/build/wamrc \
    --target=thumbv7em \
    --cpu=cortex-m7 \
    --size-level=3 \
    --enable-builtin-intrinsics=i64.common,fp.common \
    -o module.aot \
    module.wasm

echo "AOT module size: $(wc -c < module.aot) bytes"

# Convert to C header using xxd
echo "Step 3: Embedding AOT in C header..."
xxd -i module.aot > module_aot.h

echo ""
echo "================================"
echo "Plugin build complete!"
echo "================================"
echo "Generated files:"
echo "  - module.wasm ($(wc -c < module.wasm) bytes)"
echo "  - module.aot ($(wc -c < module.aot) bytes)"
echo "  - module_aot.h (embedded)"
echo ""
