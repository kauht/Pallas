EXE = pallas
RELEASE_DIR = ./build/release
DEBUG_DIR = ./build/debug
BUILD_TYPE ?= Debug

ifeq ($(BUILD_TYPE),Release)
    BUILD_DIR = $(RELEASE_DIR)
else
    BUILD_DIR = $(DEBUG_DIR)
endif

.PHONY: all build build-release build-debug run debug clean format

all: build

build:
	@cmake -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	@cmake --build $(BUILD_DIR)

build-release:
	@$(MAKE) build BUILD_TYPE=Release

build-debug:
	@$(MAKE) build BUILD_TYPE=Debug

run: build-release
	@$(RELEASE_DIR)/$(EXE)

debug: build-debug
	@$(DEBUG_DIR)/$(EXE)

clean:
	@if exist build rmdir /s /q build
	@if exist $(EXE) del $(EXE)
	@if exist pallas del pallas

format:
	@python scripts/format.py
