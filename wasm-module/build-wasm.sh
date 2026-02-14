#!/bin/bash
set -e

WAMR_ROOT=../wasm-micro-runtime

echo "Building WASM module..."

# Clean old build artifacts
rm -f module.wasm module.aot module_aot.h

# Create build directory
mkdir -p build

# Check for emcc
if ! command -v emcc &> /dev/null; then
    echo "ERROR: emscripten not found!"
    echo "Please install with: brew install emscripten"
    exit 1
fi

echo "Using emscripten: $(which emcc)"

# Compile math import stubs (declares wrapped math funcs as WASM imports)
echo "Step 1a: Compiling math import stubs..."
emcc -c -O0 math_imports.c -o build/math_imports.o

# Compile C++ to WASM using emscripten
echo "Step 1b: Compiling C++ to WASM..."
emcc \
    -O3 \
    -ffast-math \
    -sSTANDALONE_WASM \
    -sEXPORTED_RUNTIME_METHODS=[] \
    -sEXPORTED_FUNCTIONS=_process \
    -sERROR_ON_UNDEFINED_SYMBOLS=0 \
    --no-entry \
    -I RTNeural \
    -I RTNeural/modules \
    -I RTNeural/modules/rt-nam \
    -I RTNeural/modules/Eigen \
    -DRTNEURAL_DEFAULT_ALIGNMENT=8 \
    -DRTNEURAL_NO_DEBUG=1 \
    -DRTNEURAL_USE_EIGEN=1 \
    -Wl,--wrap=tanhf -Wl,--wrap=tanh \
    -Wl,--wrap=expf -Wl,--wrap=exp \
    -Wl,--wrap=logf -Wl,--wrap=log \
    -Wl,--wrap=sinf -Wl,--wrap=cosf -Wl,--wrap=tanf \
    -Wl,--wrap=sin -Wl,--wrap=cos -Wl,--wrap=tan \
    -o build/module.wasm \
    build/math_imports.o module.cpp

echo "WASM module size: $(wc -c < build/module.wasm) bytes"

# Check for wamrc and build if missing
if [ ! -f "$WAMR_ROOT/wamr-compiler/build/wamrc" ]; then
    echo ""
    echo "wamrc not found at $WAMR_ROOT/wamr-compiler/build/wamrc"
    echo "Building wamrc AOT compiler (this takes a few minutes)..."
    pushd $WAMR_ROOT/wamr-compiler > /dev/null
    # build_llvm.sh handles LLVM cloning and building (has internal idempotency checks)
    echo "Building LLVM dependencies..."
    ./build_llvm.sh
    # Build wamrc
    mkdir -p build
    cd build
    echo "Configuring wamrc..."
    cmake ..
    echo "Building wamrc..."
    make -j$(sysctl -n hw.ncpu)
    popd > /dev/null
    echo "wamrc build complete!"
fi

# Compile WASM to AOT for Cortex-M7
# Note: bulk memory is enabled by default in wamrc
echo "Step 2: Compiling WASM to AOT..."
$WAMR_ROOT/wamr-compiler/build/wamrc \
    --target=thumbv7em \
    --cpu=cortex-m7 \
    --opt-level=3 \
    --size-level=0 \
    -o build/module.aot \
    build/module.wasm

echo "AOT module size: $(wc -c < build/module.aot) bytes"

# Convert to C header using xxd
echo "Step 3: Embedding AOT in C header..."
xxd -i -n module_aot build/module.aot > build/module_aot.h

echo ""
echo "================================"
echo "Module build complete!"
echo "================================"
echo "Generated files:"
echo "  - build/module.wasm ($(wc -c < build/module.wasm) bytes)"
echo "  - build/module.aot ($(wc -c < build/module.aot) bytes)"
echo "  - build/module_aot.h (embedded)"
echo ""
