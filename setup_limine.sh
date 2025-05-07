#!/bin/bash

set -e  # Exit on error

# Define paths
LIMINE_DIR="limine"

# Step 1: Remove any existing Limine directory
echo "[*] Removing old Limine version if it exists..."
rm -rf "$LIMINE_DIR"

# Step 2: Clone the correct Limine release
echo "[*] Cloning Limine v9.x-binary..."
git clone --branch v9.x-binary --depth 1 https://github.com/limine-bootloader/limine.git "$LIMINE_DIR"

# Step 3: Ensure required directories exist
echo "[*] Creating required directories..."
mkdir -p "iso_root/boot/limine"
mkdir -p "iso_root/EFI/BOOT"

# Step 4: Copy Limine bootloader components
echo "[*] Copying Limine binaries..."
cp "$LIMINE_DIR/limine-bios.sys" "iso_root/boot/limine/"
cp "$LIMINE_DIR/limine-bios-cd.bin" "iso_root/boot/limine/"
cp "$LIMINE_DIR/limine-bios-pxe.bin" "iso_root/"
cp "$LIMINE_DIR/limine-uefi-cd.bin" "iso_root/boot/limine/"
cp "$LIMINE_DIR/BOOTIA32.EFI" "iso_root/EFI/BOOT/"
cp "$LIMINE_DIR/BOOTX64.EFI" "iso_root/EFI/BOOT/"

echo "[+] Limine v9.x-binary setup complete!"
