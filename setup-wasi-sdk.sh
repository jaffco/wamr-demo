#!/bin/bash
# Quick setup script to download and install WASI SDK

set -e

WASI_VERSION="24"
WASI_FULL_VERSION="24.0"

echo "================================"
echo "WASI SDK Setup for macOS"
echo "================================"
echo ""

# Detect architecture
ARCH=$(uname -m)
if [ "$ARCH" = "arm64" ]; then
    WASI_PLATFORM="macos-arm64"
else
    WASI_PLATFORM="macos"
fi

WASI_TARBALL="wasi-sdk-${WASI_FULL_VERSION}-${WASI_PLATFORM}.tar.gz"
WASI_URL="https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_VERSION}/${WASI_TARBALL}"

echo "Detected platform: $WASI_PLATFORM"
echo "Will download: $WASI_TARBALL"
echo ""

# Check if already installed
if [ -d "/opt/wasi-sdk" ]; then
    echo "WASI SDK already installed at /opt/wasi-sdk"
    read -p "Reinstall? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "Keeping existing installation"
        exit 0
    fi
    sudo rm -rf /opt/wasi-sdk
fi

# Download
echo "Downloading WASI SDK..."
cd /tmp
curl -L -o "$WASI_TARBALL" "$WASI_URL"

# Extract
echo "Extracting..."
tar xzf "$WASI_TARBALL"

# Install
echo "Installing to /opt/wasi-sdk (requires sudo)..."
sudo mv "wasi-sdk-${WASI_FULL_VERSION}" /opt/wasi-sdk

# Cleanup
rm "$WASI_TARBALL"

echo ""
echo "================================"
echo "WASI SDK installed successfully!"
echo "================================"
echo "Location: /opt/wasi-sdk"
echo ""
echo "You can now build the WASM module with:"
echo "  cd wasm-module"
echo "  ./build-wasm.sh"
