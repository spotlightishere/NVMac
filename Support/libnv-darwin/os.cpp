//
//  os.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <DriverKit/IOMemoryMap.h>
#include <PCIDriverKit/PCIDriverKit.h>
#include <cstdlib>
#include <map>
#include <time.h>

// TODO(spotlightishere): There's also no way this will scale
std::map<void*, IOMemoryMap*> kernel_space_map{};

extern "C" {
// NVIDIA has their own function called `os_log_error`.
// Undefine the Apple-provided macro before importing.
#undef os_log_error
#include "os-interface.h"

#pragma mark - Support checks

NvBool os_is_grid_supported(void) {
    return NV_FALSE;
}

NvU32 os_get_grid_csp_support(void) {
    return 0;
}

NV_STATUS os_get_is_openrm(NvBool* bIsOpenRm) {
    // Sure are!
    *bIsOpenRm = NV_TRUE;
    return NV_OK;
}

NvBool os_pat_supported(void) {
    // Not implemented
    return NV_FALSE;
}

NvBool os_is_efi_enabled(void) {
    // We always have EFI!
    return NV_TRUE;
}

#pragma mark - Hardware

NvBool os_dma_buf_enabled = NV_FALSE;

// We do not support confidental compute.
NvBool os_cc_enabled = 0;
NvBool os_cc_sev_snp_enabled = 0;
NvBool os_cc_snp_vtom_enabled = 0;
NvBool os_cc_tdx_enabled = 0;
NvBool os_cc_sme_enabled = 0;

NvBool nv_requires_dma_remap(nv_state_t* nv) {
    return NV_FALSE;
}

nv_state_t* nv_get_ctl_state(void) {
    return NV_STATE_PTR;
}

NvU32 os_get_cpu_number(void) {
    // TODO(spotlightishere): Implement, if necessary
    return 1;
}

NvU32 os_get_cpu_count(void) {
    // TODO(spotlightishere): Implement, if necessary
    return 1;
}

NvU64 os_get_cpu_frequency(void) {
    // TODO(spotlightishere): Implement
    return 1;
}

NV_STATUS os_registry_init(void) {
    return NV_OK;
}

void os_disable_console_access(void) {
    // Stubbed
}

void os_enable_console_access(void) {
    // Stubbed
}

NV_STATUS os_device_vm_present(void) {
    return NV_ERR_NOT_SUPPORTED;
}

NvBool os_is_nvswitch_present(void) {
    // TODO: Implement if desired
    return NV_FALSE;
}

NvBool os_is_vgx_hyper(void) {
    return NV_FALSE;
}

NV_STATUS os_inject_vgx_msi(NvU16 guestID, NvU64 msiAddr, NvU32 msiData) {
    return NV_ERR_NOT_SUPPORTED;
}

// TODO: This should probably be supported some day.
NvBool nv_is_chassis_notebook(void) {
    return NV_FALSE;
}

void nv_set_dma_address_size(nv_state_t* nv, NvU32 phys_addr_bits) {
    // TODO(spotlightishere): We should support DMA
    nvd_log("Stubbed: nv_set_dma_address_size");
    return;
}

#pragma mark - Time

NV_STATUS os_get_current_time(NvU32* seconds, NvU32* useconds) {
    uint64_t nanoseconds = clock_gettime_nsec_np(CLOCK_REALTIME);
    NvU32 microseconds = (NvU32)nanoseconds / 1000;
    *useconds = microseconds;
    *seconds = microseconds / 1000000;
    return NV_OK;
}

NvU64 os_get_current_tick_hr(void) {
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}

NvU64 os_get_current_tick(void) {
    return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
}

NV_STATUS os_delay_us(NvU32 MicroSeconds) {
    IOSleep(MicroSeconds);
    return NV_OK;
}

NV_STATUS os_delay(NvU32 MilliSeconds) {
    IOSleep(MilliSeconds * 1000);
    return NV_OK;
}

#pragma mark - Process Info

// TODO(spotlightishere): Implement; threadId should be pid
NV_STATUS os_get_current_thread(NvU64* threadId) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS os_get_euid(NvU32* pSecToken) {
    return NV_ERR_NOT_SUPPORTED;
}

NvU64 os_get_max_user_va(void) {
    // TODO(spotlightishere): Get from task_info
    return 0;
}

NvU32 os_get_current_process(void) {
    // TODO(spotlightishere): Get from task_info
    return 0;
}

void os_get_current_process_name(char* buf, NvU32 len) {
    // TODO(spotlightishere): Get from task_info
    return;
}

void* os_get_pid_info(void) {
    // TODO(spotlightishere): Implement
    return NULL;
}

NvBool os_is_init_ns(void) {
    // Namespaces on XNU? Not quite.
    return NV_TRUE;
}

NvBool os_is_administrator(void) {
    // TODO: Actually implement
    return NV_TRUE;
}

NvBool os_check_access(RsAccessRight accessRight) {
    // TODO: Actually Implement
    return NV_TRUE;
}

NvBool os_is_isr(void) {
    return NV_FALSE;
}

#pragma mark - Memory

// TODO(spotlightishere): This is hardcoded
NvU32 os_page_size = 4096;
NvU64 os_page_mask = ~(os_page_size - 1);
NvU8 os_page_shift = 12;

NvU32 nv_get_dev_minor(nv_state_t* nv) {
    // TODO(spotlightishere): Figure this out
    return 0;
}

NV_STATUS os_get_page(NvU64 address) {
    return NV_ERR_NOT_SUPPORTED;
}

NvU32 os_get_page_refcount(NvU64 address) {
    return 0;
}

NV_STATUS os_get_numa_node_memory_usage(NvS32 node_id, NvU64* free_memory_bytes,
                                        NvU64* total_memory_bytes) {
    // TODO(spotlightishere): I don't even know what a NUMA node is
    *free_memory_bytes = 0;
    *total_memory_bytes = 0;
    return NV_OK;
}

NV_STATUS os_get_random_bytes(NvU8* bytes, NvU16 numBytes) {
    arc4random_buf(bytes, numBytes);
    return NV_OK;
}

NV_STATUS os_get_smbios_header(NvU64* pSmbsAddr) {
    return NV_ERR_NOT_SUPPORTED;
}

NvU64 os_get_tick_resolution(void) {
    // TODO(spotlightishere): Get from Mach
    return 1;
}

NV_STATUS os_get_version_info(os_version_info* pOsVersionInfo) {
    NV_STATUS status = NV_OK;

    // TODO(spotlightishere): Implement
    pOsVersionInfo->os_major_version = 25;
    pOsVersionInfo->os_minor_version = 5;
    pOsVersionInfo->os_build_number = 1;
    //    pOsVersionInfo->os_build_version_str
    //    pOsVersionInfo->os_build_date_plus_str

    return status;
}

void* os_map_kernel_space(NvU64 start, NvU64 size_bytes, NvU32 mode) {
    // Our start contains our memoryIndex.
    uint8_t memoryIndex = (uint8_t)start & 0xFF;

    nvd_log("[DEBUG] Mapping at memory index %hhu with size %llu", memoryIndex,
            size_bytes);
    IOPCIDevice* device = (IOPCIDevice*)nvd_state->device;

    IOMemoryDescriptor* returnMemory = NULL;
    kern_return_t result = device->_CopyDeviceMemoryWithIndex(
        memoryIndex, &returnMemory, ((IOService*)nvd_state->service));
    if (result != KERN_SUCCESS) {
        nvd_log("Failed to map memory: %d", result);
        // TODO: This should be fatal
        return NULL;
    }

    // Determine what option to use.
    uint64_t mappingOptions = 0;
    switch (mode) {
    case NV_MEMORY_CACHED:
        mappingOptions = kIOMemoryMapCacheModeDefault;
        break;
    case NV_MEMORY_WRITECOMBINED:
        // TODO: No idea how to support this yet
    case NV_MEMORY_UNCACHED:
    case NV_MEMORY_DEFAULT:
        mappingOptions = kIOMemoryMapCacheModeInhibit;
        break;
    default:
        nv_printf(NV_DBG_ERRORS,
                  "NVRM: os_map_kernel_space: unsupported mode!\n");
        OSSafeReleaseNULL(returnMemory);
        return NULL;
    }

    // Map this range into our space.
    IOMemoryMap* mapping = NULL;
    result = returnMemory->CreateMapping(mappingOptions, 0, 0, size_bytes, 0,
                                         &mapping);
    if (result != KERN_SUCCESS) {
        nvd_log("Failed to create memory mapping: %d", result);
        OSSafeReleaseNULL(returnMemory);
        return NULL;
    }

    // TODO(spotlightishere): Keep track of these mappings lol
    void* addressBase = (void*)mapping->GetAddress();
    kernel_space_map[addressBase] = mapping;
    nvd_log("[DEBUG] mapping at %p for size %llu", addressBase, size_bytes);
    OSSafeReleaseNULL(returnMemory);

    return addressBase;
}

void os_unmap_kernel_space(void* addressBase, NvU64 size_bytes) {
    // No need to leverage byte size - we already have an IOMemoryMap
    // that will handle unmapping on our behalf.
    if (kernel_space_map.contains(addressBase) == false) {
        nvd_log(
            "TEMP panic Memory was unmapped for pointer that does not exist!");
        return;
    }

    nvd_log("unmapping %p", addressBase);

    IOMemoryMap* mapping = kernel_space_map[addressBase];
    OSSafeReleaseNULL(mapping);
    kernel_space_map.erase(addressBase);
}
}
