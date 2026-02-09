#!/bin/bash

set -e

cd ./wasm-module && ./build-wasm.sh && cd ..

make -j$(getconf _NPROCESSORS_ONLN)
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

# Process log.txt into WAV file
echo ""
echo "Processing log.txt into impulse_response.wav..."
python3 -c "
import wave, struct, re, sys

samples = []
with open('log.txt', 'r') as f:
    for line in f:
        m = re.search(r'S:(-?\d+)', line)
        if m:
            samples.append(int(m.group(1)) / 100000.0)

if not samples:
    print('No audio samples found in log.txt')
    sys.exit(1)

print(f'Found {len(samples)} samples ({len(samples)/48000:.1f}s at 48kHz)')

with wave.open('impulse_response.wav', 'w') as wav:
    wav.setnchannels(1)
    wav.setsampwidth(2)
    wav.setframerate(48000)
    for s in samples:
        s = max(-1.0, min(1.0, s))
        wav.writeframes(struct.pack('<h', int(s * 32767)))

print(f'Written impulse_response.wav')
"