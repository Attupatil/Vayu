#!/bin/bash
set -e

# Configuration
DISTRO="debian"
RELEASE="bookworm"
ARCH="amd64"
ROOTFS="./build/rootfs"
HOSTNAME="VayuOS"

echo "=== Vayu OS Bootstrap Phase ==="

# 1. Clean and Prepare
mkdir -p $ROOTFS

# 2. Debootstrap
echo "Bootstrapping minimal $RELEASE system..."
debootstrap --arch=$ARCH $RELEASE $ROOTFS http://deb.debian.org/debian/

# 3. Configure Basic System
echo "Configuring hostname..."
echo "$HOSTNAME" > $ROOTFS/etc/hostname

# 4. Install Essential Packages
echo "Installing kernel and system utilities..."
chroot $ROOTFS apt-get update
chroot $ROOTFS apt-get install -y --no-install-recommends \
    linux-image-amd64 systemd kmod pciutils usbutils bash coreutils \
    network-manager pipewire pipewire-audio-client-libraries \
    lightdm xserver-xorg-core xserver-xorg-video-vmware \
    open-vm-tools open-vm-tools-desktop \
    lxqt-core terminal-emulator firefox-esr \
    supertux aisleriot gnome-mines

# 5. User Configuration
echo "Configuring users..."
chroot $ROOTFS useradd -m -s /bin/bash vayu
echo "vayu:vayu" | chroot $ROOTFS chpasswd
chroot $ROOTFS usermod -aG sudo vayu

# 6. Auto-login Configuration
echo "Configuring auto-login..."
cat <<EOF > $ROOTFS/etc/lightdm/lightdm.conf.d/vayu-autologin.conf
[Seat:*]
autologin-user=vayu
autologin-user-timeout=0
EOF

echo "Bootstrap complete."
