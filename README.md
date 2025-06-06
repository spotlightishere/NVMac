# NVMac

A running attempt to port the NVIDIA open source kernel modules for Linux to macOS.

There are two usable targets here:
 - `PCIHorrorKit`, a PCI DriverKit extension.
 - `NVMacRM`, a standard kernel extension.
 
 Development has primarily occurred within `PCIHorror` due to the ease of development.

This project additionally contains several libraries:
 - `libnv-kernel`, build support for the OS-independent NVIDIA library within the kernel module (`Support/open-gpu-kernel-modules/src/nvidia`).
 - `libnv-darwin`, implementing Darwin-specific functionality for the NVIDIA driver to use.
 - `libspdm-polyfill`, a polyfill for NVIDIA-specific extensions to libspdm.

## Why?
I just want my GPU's fans to not run at 100%.
