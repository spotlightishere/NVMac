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

void* nvlink_malloc(NvLength size) {
    return IOMalloc(size);
}

void nvlink_free(void* ptr) {
    // TODO(spotlightishere): Implement
    // We need to use IOFree.
    // It requires the length of the deallocation,
    // so we would need to keep track of allocations
    // performed by NVLink.
    //
    // For now, we'll put a size of zero.
    // (This should panic.)
    IOFree(ptr, 0);
    return;
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
