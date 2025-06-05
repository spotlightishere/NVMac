//
//  libc.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <cstdlib>
#include <string>

extern "C" {

#pragma mark - OS functions

NvS32 os_mem_cmp(const NvU8* s1, const NvU8* s2, NvU32 n) {
    return memcmp(s1, s2, n);
}

static void* os_mem_copy(void* dst, const void* src, NvU32 length) {
    return memcpy(dst, src, length);
}

void* os_mem_set(void* dst, NvU8 c, NvU32 length) {
    return memset(dst, c, length);
}

// TODO(spotlightishere): Implement
// There are dedicated vm_xxx functions we can use.
static void* os_memcpy_from_user(void* dst, const void* src, NvU32 length) {
    return memcpy(dst, src, length);
}

// TODO(spotlightishere): Implement
// There are dedicated vm_xxx functions we can use.
static void* os_memcpy_to_user(void* dst, const void* src, NvU32 length) {
    return memcpy(dst, src, length);
}
}
