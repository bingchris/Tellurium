#!/bin/bash

set -e  # Exit immediately if a command fails
BUILD_DIR=".build"
LIMINE_REPO="https://github.com/limine-bootloader/limine.git"

# Handle "clean" argument
if [[ "$1" == "clean" ]]; then
    echo "[+] Cleaning up '$BUILD_DIR'..."
    rm -rf "$BUILD_DIR"
    echo "[+] Cleanup complete!"
    exit 0
fi

# Create the .build directory
mkdir -p "$BUILD_DIR"

echo "[+] Cloning Limine repository..."
git clone --depth=1 "$LIMINE_REPO" "$BUILD_DIR/limine"

cd "$BUILD_DIR/limine"

echo "[+] Building Limine..."
make -C test -f test.mk clean
make -C test -f test.mk

echo "[+] Copying necessary files..."
mkdir -p "$BUILD_DIR"
cp bin/limine-bios.bin bin/limine-efi.bin bin/limine.sys "$BUILD_DIR/"
cp -r config "$BUILD_DIR/"

echo "[+] Limine setup completed!"
echo "Bootloader files are now in '$BUILD_DIR'."
