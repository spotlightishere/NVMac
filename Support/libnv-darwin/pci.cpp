//
//  pci.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"
#include <PCIDriverKit/PCIDriverKit.h>

// Taken from the Linux kernel module source.
#define NV_PCIE_CFG_MAX_OFFSET 0x1000

extern "C" {

void* os_pci_init_handle(NvU32 domain, NvU8 bus, NvU8 slot, NvU8 function,
                         NvU16* vendor, NvU16* device) {
    // TODO(spotlightishere): We currently orient ourselves
    // around a single NVIDIA GPU.
    // As such, we already have an IOPCIDevice to work with.
    //
    // This should be rearchitectured to handle multiple GPUs
    // if such is desired in the future.
    NV_GLOBAL_DEVICE->ConfigurationRead16(kIOPCIConfigurationOffsetVendorID,
                                          vendor);
    NV_GLOBAL_DEVICE->ConfigurationRead16(kIOPCIConfigurationOffsetDeviceID,
                                          device);
    return (void*)NV_GLOBAL_DEVICE;
}

NvBool os_pci_remove_supported(void) {
    // We have no way to remove ourselves.
    // TODO: It'd be nice to handle removal for e.g. Thunderbolt.
    return NV_FALSE;
}

void os_pci_remove(void* handle) {
    // Stubbed as we have no way to remove ourselves.
    return;
}

NV_STATUS os_pci_read_byte(void* handle, NvU32 offset, NvU8* pReturnValue) {
    if (offset >= NV_PCIE_CFG_MAX_OFFSET) {
        *pReturnValue = UINT8_MAX;
        return NV_ERR_NOT_SUPPORTED;
    }

    NV_GLOBAL_DEVICE->MemoryRead8(0, offset, pReturnValue);
    return NV_OK;
}

NV_STATUS os_pci_write_byte(void* handle, NvU32 offset, NvU8 value) {
    if (offset >= NV_PCIE_CFG_MAX_OFFSET)
        return NV_ERR_NOT_SUPPORTED;

    NV_GLOBAL_DEVICE->MemoryWrite8(0, offset, value);
    return NV_OK;
}

NV_STATUS os_pci_read_word(void* handle, NvU32 offset, NvU16* pReturnValue) {
    if (offset >= NV_PCIE_CFG_MAX_OFFSET) {
        *pReturnValue = 0xFF;
        return NV_ERR_NOT_SUPPORTED;
    }

    NV_GLOBAL_DEVICE->MemoryRead16(0, offset, pReturnValue);
    return NV_OK;
}

NV_STATUS os_pci_write_word(void* handle, NvU32 offset, NvU16 value) {
    if (offset >= NV_PCIE_CFG_MAX_OFFSET)
        return NV_ERR_NOT_SUPPORTED;

    NV_GLOBAL_DEVICE->MemoryWrite16(0, offset, value);
    return NV_OK;
}

NV_STATUS os_pci_read_dword(void* handle, NvU32 offset, NvU32* pReturnValue) {
    if (offset >= NV_PCIE_CFG_MAX_OFFSET) {
        *pReturnValue = UINT32_MAX;
        return NV_ERR_NOT_SUPPORTED;
    }

    NV_GLOBAL_DEVICE->MemoryRead32(0, offset, pReturnValue);
    return NV_OK;
}

NV_STATUS os_pci_write_dword(void* handle, NvU32 offset, NvU32 value) {
    if (offset >= NV_PCIE_CFG_MAX_OFFSET)
        return NV_ERR_NOT_SUPPORTED;

    NV_GLOBAL_DEVICE->MemoryWrite32(0, offset, value);
    return NV_OK;
}
}
