#!/bin/bash
set -e

# Configuration
ROOTFS="./build/rootfs"
ISO_DIR="./build/iso_root"
ISO_NAME="Vayu-x86_64.iso"

echo "=== Vayu OS ISO Build Phase ==="

# 1. Prepare ISO Root
rm -rf $ISO_DIR
mkdir -p $ISO_DIR/live
mkdir -p $ISO_DIR/boot/grub

# 2. Compress Rootfs
echo "Creating squashfs..."
mksquashfs $ROOTFS $ISO_DIR/live/filesystem.squashfs -comp xz -e boot

# 3. Copy Kernel and Initrd
echo "Copying kernel and initrd..."
cp $ROOTFS/boot/vmlinuz-* $ISO_DIR/boot/vmlinuz
cp $ROOTFS/boot/initrd.img-* $ISO_DIR/boot/initrd

# 4. Configure GRUB
echo "Configuring GRUB..."
cat <<EOF > $ISO_DIR/boot/grub/grub.cfg
set default=0
set timeout=5

menuentry "Vayu OS (Live)" {
    linux /boot/vmlinuz boot=live quiet splash
    initrd /boot/initrd
}
EOF

# 5. Build ISO
echo "Building ISO with xorriso..."
grub-mkrescue -o $ISO_NAME $ISO_DIR

mv $ISO_NAME ./build/

echo "ISO build complete: ./build/$ISO_NAME"
