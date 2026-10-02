# VMware Workstation Setup Guide for Vayu OS

To get the best experience with Vayu OS on VMware Workstation, follow these steps:

## 1. Create a New Virtual Machine
- **Type:** Typical (recommended)
- **Installer Disk Image:** Select `Vayu-x86_64.iso`.
- **Guest OS:** Linux -> Debian 12.x 64-bit.
- **VM Name:** Vayu OS.

## 2. Hardware Configuration
- **Memory:** Minimum 1GB (2GB recommended).
- **Processors:** 2 cores.
- **Graphics:** Accelerate 3D graphics (if supported by host).

## 3. Post-Boot Integration
Vayu OS comes pre-installed with `open-vm-tools-desktop`. Once booted:
- **Resolution Scaling:** The desktop should automatically scale to fit the window.
- **Mouse Tracking:** Seamless mouse movement between host and guest is enabled by default.
- **Copy-Paste:** Shared clipboard should be functional.

## 4. Troubleshooting
- If the screen doesn't resize, try toggling "Fit Guest Now" in the VMware View menu.
- Ensure "Virtualize Intel VT-x/EPT or AMD-V/RVI" is enabled in Processor settings if you plan to run nested containers.
