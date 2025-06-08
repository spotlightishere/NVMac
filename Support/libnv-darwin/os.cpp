//
//  os.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <cstdlib>
#include <string>
#include <time.h>

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

void* os_get_pid_info(void) {
    // This is used for PID resolution within
    // namespaces, which we do not support.
    return NULL;
}

void os_put_pid_info(void* pid_info) {
    // No-op - we did not allocate anything above.
    return;
}

NV_STATUS os_find_ns_pid(void* pid_info, NvU32* ns_pid) {
    // We have no concept of namespaces in XNU.
    *ns_pid = os_get_current_process();
    return NV_OK;
}

NvU32 os_get_current_process(void) {
    int32_t pid = 0;
    pid_for_task(nvd_task(), &pid);
    return pid;
}

NvBool os_is_init_ns(void) {
    // We have no concept of namespaces in XNU.
    return NV_TRUE;
}

NV_STATUS os_get_current_thread(NvU64* threadId) {
    // This API makes more sense under Linux, where
    // this returns the process's actual PID.
    *threadId = os_get_current_process();
    return NV_OK;
}

NV_STATUS os_get_euid(NvU32* pSecToken) {
#if TARGET_OS_DRIVERKIT
    *pSecToken = geteuid();
    return NV_OK;
#else
    // TODO: For kernel code, we want to look up get_bsdtask_info.
#error Not yet implemented for kernel code.
    return NV_ERR_NOT_SUPPORTED;
#endif
}

void os_get_current_process_name(char* buf, NvU32 len) {
    // Obtain from the current process's PID.
    proc_name_t current_name;
    int32_t pid = os_get_current_process();
    proc_name(pid, current_name, sizeof(proc_name_t));

    // Copy to client.
    strncpy(buf, current_name, len);
    return;
}

NvU64 os_get_max_user_va(void) {
    // TODO(spotlightishere): This may not be
    // something we can truly handle.
    return 0;
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

// Stubbed under Linux.
void os_add_record_for_crashLog(void* pbuffer, NvU32 size) {
    return;
}

// Stubbed under Linux.
void os_delete_record_for_crashLog(void* pbuffer) {
    return;
}

#pragma mark - Memory

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

NV_STATUS os_flush_cpu_cache_all(void) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS os_flush_user_cache(void) {
    return NV_ERR_NOT_SUPPORTED;
}

// Taken from the Linux kernel module.
void os_flush_cpu_write_combine_buffer(void) {
#if defined(NVCPU_X86_64)
    asm volatile("sfence" ::: "memory");
#elif defined(NVCPU_AARCH64)
    asm volatile("dsb st" : : : "memory");
#else
#error Not sure how to flush memory
#endif
}
}
