#!/bin/bash

cd ./wasm-module && ./build-wasm.sh && cd ..

# NOTE- if just changing wasm, make clean not needed.
# if changing wasm and main.cpp, make clean also not needed.

make
make program-dfu

# https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment#4a-flashing-the-daisy-via-usb
echo "Ignore Error 74 if related to download get_status"