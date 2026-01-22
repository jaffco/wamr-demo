# Project Name
TARGET = main

# Sources
CPP_SOURCES = src/main.cpp

# WASM Module - Build before main compilation
WASM_MODULE_DIR = wasm-module
WASM_MODULE_HEADER = $(WASM_MODULE_DIR)/build/module_aot.h

# Include WAMR runtime build first (before common.mk processes C_SOURCES)
include wamr.mk

# Library Locations
include common.mk

# Ensure module is built before compilation
.PHONY: build-module
build-module:
	@echo "Building WASM module..."
	@cd $(WASM_MODULE_DIR) && bash build.sh

$(WASM_MODULE_HEADER): build-module
	@touch $(WASM_MODULE_HEADER)
# Force module build before main target
all: build-module build/$(TARGET).bin

