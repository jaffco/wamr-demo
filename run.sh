#!/bin/bash

set -e

cd ./wasm-module && ./build-wasm.sh && cd ..

make
make program-dfu

# https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment#4a-flashing-the-daisy-via-usb
echo "Ignore Error 74 if related to download get_status"

# Wait for device to reconnect after flashing
sleep 1

# Auto-detect the USB modem device
DEVICE=$(ls /dev/tty.usbmodem* 2>/dev/null | head -1)
if [ -z "$DEVICE" ]; then
  echo "No USB modem device found. Please ensure the Daisy device is connected."
  exit 1
fi
echo "Connecting to device: $DEVICE"

# Connect to the device and log output
script log.txt screen $DEVICE 115200