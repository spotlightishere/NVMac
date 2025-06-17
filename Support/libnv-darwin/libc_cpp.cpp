//
//  libc_cpp.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <map>

// TODO(spotlightishere): There's no way this will scale
static std::map<void*, uint64_t> global_allocations{};

extern "C" {

#pragma mark - Memory Management

NV_STATUS os_alloc_mem(void** address, NvU64 size) {
    void* ptr = IOMallocZero(size);
    if (ptr == NULL) {
        return NV_ERR_NO_MEMORY;
    }

    global_allocations[ptr] = size;
    *address = ptr;
    return NV_OK;
}

void os_free_mem(void* ptr) {
    if (global_allocations.contains(ptr) == false) {
        nvd_log(
            "TEMP panic Allocation was freed for pointer that does not exist!");
        return;
    }

    uint64_t allocation_size = global_allocations[ptr];
    IOFree(ptr, allocation_size);

    global_allocations.erase(ptr);
}
}
