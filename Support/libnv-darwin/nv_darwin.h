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
#else
#include <IOKit/IOLib.h>
#endif

#include <os/log.h>

#ifdef __cplusplus
extern "C" {
#endif

// libnv-kernel uses C linkage.
#include <nv.h>

#ifdef __cplusplus
}
#endif

typedef struct nv_darwin_state_s {
    nv_state_t nv_state;
} nv_darwin_state_t;

/// Helper macro to allow accessing the global `nv_state_t`
/// within our global `nv_darwin_state_t.`
#define NV_STATE_PTR(nvd) &(((nv_darwin_state_t*)(nvd))->nv_state)

/// Helper macro to allow logging via the default `os_log`.
#define nvd_log(fmt, ...) os_log(OS_LOG_DEFAULT, (fmt), ##__VA_ARGS__)

#endif /* nv_darwin_h */
