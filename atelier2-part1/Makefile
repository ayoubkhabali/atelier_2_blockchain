# ========================================
# QUESTION 12: Makefile for test automation
# ========================================

.PHONY: all build test clean

all: build

build:
	@echo "Building project..."
	@if not exist build mkdir build
	@cd build && cmake .. -G "MinGW Makefiles" && cmake --build .

test: build
	@echo ========================================
	@echo   Running All Tests
	@echo ========================================
	@cd build && atelier2_part1.exe

clean:
	@echo "Cleaning build directory..."
	@if exist build rmdir /s /q build
	@echo "Clean complete!"

help:
	@echo Available targets:
	@echo   make build  - Build the project
	@echo   make test   - Build and run all tests
	@echo   make clean  - Remove build directory
	@echo   make help   - Show this help message
