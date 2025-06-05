//
//  nv_darwin.h
//  PCIHorrorKit
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#ifndef nv_darwin_h
#define nv_darwin_h

#include <os/log.h>

extern "C" {
#include <nv.h>
}

typedef struct nv_darwin_state_s {
    nv_state_t nv_state;
} nv_darwin_state_t;

/// Helper macro to allow accessing the global `nv_state_t`
/// within our global `nv_darwin_state_t.`
#define NV_STATE_PTR(nvd) &(((nv_darwin_state_t*)(nvd))->nv_state)

/// Helper macro to allow logging via the default `os_log`.
#define nvd_log(fmt, ...) os_log(OS_LOG_DEFAULT, (fmt), ##__VA_ARGS__)

#endif /* nv_darwin_h */
