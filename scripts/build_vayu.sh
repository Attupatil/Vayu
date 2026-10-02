#!/bin/bash

# Set non-interactive mode for Debian packages
export DEBIAN_FRONTEND=noninteractive

# Define variables
WORKSPACE=$(pwd)
ROOTFS_DIR="$WORKSPACE/rootfs"
ISO_NAME="Vayu-x86_64.iso"

# Clean workspace
rm -rf "$WORKSPACE"/*

# Bootstrap root filesystem
debootstrap --arch=amd64 bullseye "$ROOTFS_DIR" http://deb.debian.org/debian

# Install base packages
chroot "$ROOTFS_DIR" apt-get update
chroot "$ROOTFS_DIR" apt-get install -y \
    linux-image-amd64 \
    xfce4 \
    lightdm \
    network-manager \
    network-manager-gnome \
    pipewire \
    pulseaudio \
    open-vm-tools \
    open-vm-tools-desktop \
    xserver-xorg-video-vmware \
    chromium \
    xfce4-terminal \
    supertux

# Set up user
chroot "$ROOTFS_DIR" useradd -m -s /bin/bash vayu
chroot "$ROOTFS_DIR" passwd -d vayu
chroot "$ROOTFS_DIR" usermod -aG sudo vayu

# Configure LightDM for auto-login
echo "vayu vayu" >> "$ROOTFS_DIR/etc/shadow"
echo "vayu vayu" >> "$ROOTFS_DIR/etc/passwd"
echo "vayu vayu" >> "$ROOTFS_DIR/etc/group"
echo "vayu vayu" >> "$ROOTFS_DIR/etc/sudoers"
echo "vayu vayu" >> "$ROOTFS_DIR/etc/lightdm/lightdm.conf"

# Compress root filesystem
mksquashfs "$ROOTFS_DIR" "$WORKSPACE/live/filesystem.squashfs"

# Configure GRUB
cat <<EOF > "$WORKSPACE/boot/grub/grub.cfg"
menuentry "Vayu OS" {
    set root='(hd0,msdos1)'
    linux /live/vmlinuz boot=live config components quiet splash
    initrd /live/initrd.img
}
EOF

# Create ISO
xorriso -as mkisofs -o "$ISO_NAME" -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table -J -R -V "Vayu OS" "$WORKSPACE"
