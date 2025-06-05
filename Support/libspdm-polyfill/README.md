# spdm-polyfill

The Linux kernel module has components of NVIDIA-specific `libspdm` extensions leveraging the Linux crypto API (LKCA).

We simply stub this under macOS. Implementation would be easily done with CommonCrypto and various other APIs.

Please feel free to pull request implementations if this would be helpful for your project!
