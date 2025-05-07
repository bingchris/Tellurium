#!/bin/bash

set -e  # Exit immediately if a command fails
BUILD_DIR=".build"
LIMINE_REPO="https://github.com/limine-bootloader/limine.git"
FLANTERM_REPO="https://codeberg.org/mintsuki/flanterm.git"

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
git clone "$LIMINE_REPO" "$BUILD_DIR/limine"

cd "$BUILD_DIR/limine"

echo "[+] Cloning Flanterm repository..."
git clone "$FLANTERM_REPO" "common/flanterm"

echo "[+] Building Limine..."
make -C test -f test.mk clean
make -C test -f test.mk

echo "[+] Copying necessary files..."
mkdir -p "$BUILD_DIR"
cp bin/limine-bios.bin bin/limine-efi.bin bin/limine.sys "$BUILD_DIR/"
cp -r config "$BUILD_DIR/"

echo "[+] Limine setup completed!"
echo "Bootloader files are now in '$BUILD_DIR'."
