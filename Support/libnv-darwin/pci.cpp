//
//  debug.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"
#include <string>

extern "C" {

NvBool os_pci_remove_supported(void) {
    // TODO: It'd be nice to handle removal for e.g. Thunderbolt.
    return NV_FALSE;
}

void os_pci_remove(void* handle) {
    // Stubbed
    return;
}

void* os_pci_init_handle(NvU32 domain, NvU8 bus, NvU8 slot, NvU8 function,
                         NvU16* vendor, NvU16* device) {
    return NULL;
}

NV_STATUS os_pci_read_byte(void* handle, NvU32 offset, NvU8* pReturnValue) {
    return NULL;
}

NV_STATUS os_pci_write_byte(void* handle, NvU32 offset, NvU8 value) {
    return NULL;
}

NV_STATUS os_pci_read_dword(void* handle, NvU32 offset, NvU32* pReturnValue) {
    return NULL;
}

NV_STATUS os_pci_write_dword(void* handle, NvU32 offset, NvU32 value) {
    return NULL;
}

NV_STATUS os_pci_read_word(void* handle, NvU32 offset, NvU16* pReturnValue) {
    return NULL;
}

NV_STATUS os_pci_write_word(void* handle, NvU32 offset, NvU16 value) {
    return NULL;
}
}
