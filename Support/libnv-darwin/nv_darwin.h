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

typedef struct nv_darwin_state_s {
    nv_state_t nv_state;
    // Actually a PCIHorrorKit.
    void* service;
    // Actually an IOPCIDevice.
    void* device;
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

/// Helper macro to allow logging via the default `os_log`.
#define nvd_log(fmt, ...) os_log(OS_LOG_DEFAULT, (fmt), ##__VA_ARGS__)

#endif /* nv_darwin_h */
