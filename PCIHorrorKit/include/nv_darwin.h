//
//  nv_darwin.h
//  PCIHorrorKit
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#ifndef nv_darwin_h
#define nv_darwin_h

#include <os/log.h>

/// Helper macro to allow logging via the default os_log.
#define nvd_log(fmt, ...) os_log(OS_LOG_DEFAULT, (fmt), ##__VA_ARGS__)

#endif /* nv_darwin_h */
