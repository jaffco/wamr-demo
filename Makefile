# Project Name
TARGET = main

# Sources
CPP_SOURCES = src/main.cpp

# WASM Plugin - Build before main compilation
WASM_PLUGIN_DIR = wasm-plugin
WASM_PLUGIN_HEADER = $(WASM_PLUGIN_DIR)/oscillator_aot.h

# Include WAMR runtime build first (before common.mk processes C_SOURCES)
include wamr.mk

# Library Locations
include common.mk

# Ensure plugin is built before compilation
.PHONY: build-plugin
build-plugin:
	@echo "Building WASM plugin..."
	@cd $(WASM_PLUGIN_DIR) && bash build.sh

$(WASM_PLUGIN_HEADER): build-plugin
	@touch $(WASM_PLUGIN_HEADER)

# Force plugin build before main target
all: build-plugin build/$(TARGET).bin

