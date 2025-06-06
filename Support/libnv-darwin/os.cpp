//
//  os.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <DriverKit/IOLib.h>
#include <cstdlib>
#include <time.h>

extern "C" {
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

#pragma mark - Time

// TODO(spotlightishere): Implement; threadId should be pid
NV_STATUS os_get_current_thread(NvU64* threadId) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS os_get_current_time(NvU32* seconds, NvU32* useconds) {
    uint64_t nanoseconds = clock_gettime_nsec_np(CLOCK_REALTIME);
    NvU32 microseconds = (NvU32)nanoseconds / 1000;
    *useconds = microseconds;
    *seconds = microseconds / 1000000;
    return NV_OK;
}

NvU64 os_get_current_tick_hr(void) {
    return 0;
}

NvU64 os_get_current_tick(void) {
    return 0;
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

#pragma mark - Memory

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
}
