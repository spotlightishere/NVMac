//
//  nvlink.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include <DriverKit/IOLib.h>
#include <cstdlib>
#include <stdio.h>
#include <string>

#include "nv_darwin.h"

extern "C" {

// Defined within `libc.cpp`.
NV_STATUS os_alloc_mem(void** address, NvU64 size);
void os_free_mem(void* address);

void* nvlink_malloc(NvLength size) {
    void* ptr;
    os_alloc_mem(&ptr, size);
    return ptr;
}

void nvlink_free(void* ptr) {
    os_free_mem(ptr);
}

void nvlink_assert(int expression) {
    // TODO(spotlightishere): Should this instead be a panic?
    nvd_log("NVLink: Assertion failed!\n");
}

void nvlink_memcpy(void* dst, const void* src, NvLength len) {
    memcpy(dst, src, len);
}

void* nvlink_memset(void* dest, int value, NvLength size) {
    return memset(dest, value, size);
}

void nvlink_sleep(unsigned int ms) {
    IOSleep(ms);
}

NvLength nvlink_strlen(const char* s) {
    return strlen(s);
}
}
