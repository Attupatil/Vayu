# Vayu OS
![License](https://img.shields.io/badge/License-MIT-blue.svg)
![Architecture](https://img.shields.io/badge/Architecture-x86__64-orange.svg)

> **Vayu — Lightweight, modular, and invisible. Perfect for an embedded or minimal footprint scratch OS.**

Vayu is a high-performance, minimal footprint operating system designed for x86_64 architecture, optimized for VMware Workstation on ASUS hardware. It aims to be "invisible" while providing a robust base for embedded applications and minimal desktop environments.
<img width="760" height="442" alt="image" src="https://github.com/user-attachments/assets/7e11217c-89a3-43e5-bf9f-eeb6bff0225a" />

<img width="842" height="523" alt="image" src="https://github.com/user-attachments/assets/42b63d5e-455f-4895-954d-f2cfc3720681" />

<img width="1720" height="882" alt="image" src="https://github.com/user-attachments/assets/22527d4b-c6c8-4cd1-820a-aa2a07c77b16" />

<img width="1713" height="878" alt="image" src="https://github.com/user-attachments/assets/d0c999da-6d9a-42fe-8fe4-88b90db241a0" />

<img width="1716" height="873" alt="image" src="https://github.com/user-attachments/assets/798615ca-603d-4a81-b6f9-5c38014d1226" />


## Features Matrix
- **Hypervisor Optimized:** Full integration with VMware Guest Tools for seamless interaction.
- **Low Footprint:** Stripped-down kernel and rootfs for maximum efficiency.
- **Modern Web Access:** Optimized Chromium/Firefox for low RAM usage.
- **Modular GUI:** Minimalist graphical environment (LXQt/XFCE) without bloat.
- **Entertainment:** Pre-installed lightweight games and WebGL portal access.

## Architecture
Vayu is built using a debootstrap-based process, targeting a minimal systemd-managed Linux environment.

### Diagram (Mermaid)
```mermaid
graph TD
    A[Bootloader: GRUB] --> B[Kernel: Linux x86_64]
    B --> C[Init System: systemd]
    C --> D[Display Manager: LightDM]
    D --> E[Desktop Environment: Modular GUI]
    E --> F[Applications: Terminal, Browser, Games]
    C --> G[Networking: NetworkManager]
    C --> H[Audio: PipeWire]
```

## How to Build
To build Vayu OS, you need a Linux environment with `xorriso`, `squashfs-tools`, and `grub-common`.

1. Clone the repository:
   ```bash
   git clone https://github.com/Attupatil/Vayu.git
   cd Vayu
   ```
2. Run the build script:
   ```bash
   make all
   ```
3. The output will be `Vayu-x86_64.iso`.

## Installation
Detailed installation instructions will be added to the `docs/` folder.

## License
Vayu OS is released under a custom license. It is free for educational and knowledge purposes. Corporate use is permitted for testing only; commercial use is prohibited without explicit permission. See [LICENSE](LICENSE) for details.
