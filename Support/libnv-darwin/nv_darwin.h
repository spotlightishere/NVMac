//
//  nv_darwin.h
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#ifndef nv_darwin_h
#define nv_darwin_h

// Import the correct IOKit headers for this target.
#if TARGET_OS_DRIVERKIT
#include <DriverKit/IOLib.h>

#include "driverkit_shim.h"

#else
#include <IOKit/IOLib.h>
#endif

#include <os/log.h>

#ifdef __cplusplus
extern "C" {
#endif

// Before we import, allow NVIDIA headers to give us a firmware path.
// This will be loaded via the KEXT/DEXT's resources.
#define NV_FIRMWARE_FOR_NAME(name) "firmware/" name ".bin"

// libnv-kernel uses C linkage.
#include <nv.h>

#ifdef __cplusplus
}
#endif

#if TARGET_OS_DRIVERKIT
// In DriverKit land, we are a process external to the kernel.
// We'll use our own `mach_task_self`.
#define nvd_task mach_task_self
#else
// In actual kernel code, it's different - `mask_task_self`
// represents the entire kernel, instead of our own KEXT task.
// We'll want to instead use `current_task`.
#define nvd_task current_task
#endif

typedef struct nv_darwin_state_s {
    nv_state_t nv_state;
    // Actually a PCIHorrorKit.
    void* service;
    // Actually an IOPCIDevice.
    void* device;
    // The mask applied to DMA addresses.
    uint64_t dma_mask;
}* nv_darwin_state_t;

extern nv_darwin_state_t nvd_state;

/// Helper macro to allow accessing the global `nv_state_t`
/// within our global `nv_darwin_state_t.`
#define NV_STATE_PTR &(((nv_darwin_state_t)(nvd_state))->nv_state)

/// Given that `IOPCIDevice` is a C++ class, it cannot be used in C code.
/// This macro simply casts the ambiguous `void*` to `IOPCIDevice*`.
///
/// As such, it requires that you have imported the appropriate
/// type for using `IOPCIDevice`, via `PCIDriverKit` or `IOKit` itself.
#define NV_GLOBAL_DEVICE ((IOPCIDevice*)nvd_state->device)

/// Darwin-specific implementation of `nv_dma_device`.
/// (We currently do not use it - instead, prefer `NV_GLOBAL_DEVICE`.)
struct nv_dma_device {};

/// Helper macro to allow logging via the default `os_log`.
#define nvd_log(fmt, ...) os_log(OS_LOG_DEFAULT, (fmt), ##__VA_ARGS__)

#endif /* nv_darwin_h */
