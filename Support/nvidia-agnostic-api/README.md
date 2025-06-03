# nvidia-agnostic-api

This is more of a workaround due to frustrations with Xcode's build system, and a desire to avoid vendoring the `open-gpu-kernel-modules` repository.

NVIDIA's repository is structured in a way where the bulk of code performing operations is present within the `src/nvidia` directory. This is a platform-agnostic codebase.
While building this agnostic `nv-kernel.o` object, it utilizes several linker flags not supported by the macOS `ld64` linker, such as a linker script.

We work around this by creating our own Makefile that otherwise inherits and leverages its existing build system, but simply creates a static library for later usage.
