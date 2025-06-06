//
//  libc.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <cstdlib>
#include <map>
#include <string>

// TODO(spotlightishere): There's no way this will scale
static std::map<void*, uint64_t> global_allocations{};

extern "C" {

#pragma mark - Memory Management

NV_STATUS os_alloc_mem(void** address, NvU64 size) {
    void* ptr = IOMalloc(size);
    if (ptr == NULL) {
        return NV_ERR_NO_MEMORY;
    }
    
    global_allocations[ptr] = size;
    *address = ptr;
    return NV_OK;
}

void os_free_mem(void* ptr) {
    if (global_allocations.contains(ptr) == false) {
        panic("Allocation was freed for pointer that does not exist!");
    }
    
    uint64_t allocation_size = global_allocations[ptr];
    IOFree(ptr, allocation_size);
    
    global_allocations.erase(ptr);
}

#pragma mark - Strings

NvS32 os_mem_cmp(const NvU8* s1, const NvU8* s2, NvU32 n) {
    return memcmp(s1, s2, n);
}

void* os_mem_copy(void* dst, const void* src, NvU32 length) {
    return memcpy(dst, src, length);
}

// TODO(spotlightishere): the compiler is optimizing
// all calls to `memset` to this function for some reason,
// and I am struggling to diagnose why.
//
// Instead, please take this manual version.
void* os_mem_set(void* dst, NvU8 c, NvU32 length) {
    char* tmp = (char*)dst;
    
    for (int i = 0; i < length; i++) {
        tmp[i] = c;
    }
    
    return dst;
}

char* os_string_copy(char* dst, const char* src) {
    // Hack since we have no `strcpy`
    size_t hack = strlen(dst);
    strlcpy(dst, src, hack);
    return dst;
}

NvU32 os_string_length(const char* str) {
    return (NvU32)strlen(str);
}

NvS32 os_string_compare(const char* s1, const char* s2) {
    return strcmp(s1, s2);
}

NvS32 os_snprintf(char* str, NvU32 size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    NvS32 result = snprintf(str, size, fmt, args);
    va_end(args);
    
    return result;
}

NvU32 os_strtoul(const char* str, char** endptr, NvU32 base) {
    return (NvU32)strtoul(str, endptr, base);
}

#pragma mark - Locks, mutexes

NV_STATUS os_alloc_spinlock(void** ppSpinlock) {
    void* spinlock = IOMallocZero(sizeof(os_unfair_lock_s));
    *ppSpinlock = spinlock;
    return NV_OK;
}

void os_free_spinlock(void* pSpinlock) {
    IOFree(pSpinlock, sizeof(os_unfair_lock_s));
}

// Return value are flags that we do not respect.
NvU64 os_acquire_spinlock(void* pSpinlock) {
    os_unfair_lock_lock((os_unfair_lock_t)pSpinlock);
    return 0;
}

void os_release_spinlock(void* pSpinlock, NvU64 oldIrql) {
    os_unfair_lock_unlock((os_unfair_lock_t)pSpinlock);
}
}
