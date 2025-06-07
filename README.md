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

## Building
Largely a work in progress.

Clone this repository with submodules.

You will need to download the corresponding binary driver to this firmware (as of writing, 575.57.08).
Extract it:
> ./NVIDIA-Linux-x86_64-575.57.08.run -x

This will provide you with a new folder named `NVIDIA-Linux-x86_64-575.57.08.run`, and a folder named `firmware` within.
Copy this folder to 

For DriverKit, please edit `Support/open-source-src/nvidia/arch/nvalloc/unix/src/asm/x86/nv_rdcr.c` to return 0.
Accessing the `cr4` register is not available above ring 0 (kernel), and DriverKit extensions run in userland.

You may need to comment out the `UvmPmaGpuMemoryType_tag` and callback within `src/nvidia/arch/nvalloc/unix/include/nv.h`.
(TODO: This should not be necessary.) 
