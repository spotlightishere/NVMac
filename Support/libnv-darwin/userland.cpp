//
//  userland.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"

extern "C" {

// TODO(spotlightishere): Implement
// There are dedicated vm_xxx functions we can use.
void* os_memcpy_from_user(void* dst, const void* src, NvU32 length) {
    return memcpy(dst, src, length);
}

// TODO(spotlightishere): Implement
// There are dedicated vm_xxx functions we can use.
void* os_memcpy_to_user(void* dst, const void* src, NvU32 length) {
    return memcpy(dst, src, length);
}
}
