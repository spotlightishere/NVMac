//
//  libc.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <stdio.h>
#include <string.h>

#pragma mark - Strings

NvS32 os_mem_cmp(const NvU8* s1, const NvU8* s2, NvU32 n) {
    return memcmp(s1, s2, n);
}

void* os_mem_copy(void* dst, const void* src, NvU32 length) {
    return memcpy(dst, src, length);
}

void* os_mem_set(void* dst, NvU8 c, NvU32 length) {
    return memset(dst, c, length);
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

#pragma mark - Spinlock

NV_STATUS os_alloc_spinlock(void** ppSpinlock) {
    void* spinlock = IOMallocZero(sizeof(os_unfair_lock));
    *ppSpinlock = spinlock;
    return NV_OK;
}

void os_free_spinlock(void* pSpinlock) {
    IOFree(pSpinlock, sizeof(os_unfair_lock));
}

// Return value are flags that we do not respect.
NvU64 os_acquire_spinlock(void* pSpinlock) {
    os_unfair_lock_lock((os_unfair_lock_t)pSpinlock);
    return 0;
}

void os_release_spinlock(void* pSpinlock, NvU64 oldIrql) {
    os_unfair_lock_unlock((os_unfair_lock_t)pSpinlock);
}

#pragma mark - Mutex

NV_STATUS os_alloc_mutex(void** ppMutex) {
    void* mutex = IOMalloc(sizeof(pthread_mutex_t));
    pthread_mutex_init((pthread_mutex_t*)mutex, NULL);
    *ppMutex = mutex;
    return NV_OK;
}

void os_free_mutex(void* pMutex) {
    IOFree(pMutex, sizeof(pthread_mutex_t));
}

NV_STATUS os_acquire_mutex(void* pMutex) {
    pthread_mutex_lock((pthread_mutex_t*)pMutex);
    return NV_OK;
}

NV_STATUS os_cond_acquire_mutex(void* pMutex) {
    int result = pthread_mutex_trylock((pthread_mutex_t*)pMutex);
    if (result == 0) {
        return NV_OK;
    } else {
        return NV_ERR_TIMEOUT_RETRY;
    }
}

void os_release_mutex(void* pMutex) {
    pthread_mutex_unlock((pthread_mutex_t*)pMutex);
}

#pragma mark - R/W Lock

void* NV_API_CALL os_alloc_rwlock(void)
{
    void* rwlock = IOMalloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init((pthread_rwlock_t*)rwlock, NULL);
    return rwlock;
}

void os_free_rwlock(void *pRwLock) {
    IOFree(pRwLock, sizeof(pthread_rwlock_t));
}

NV_STATUS os_acquire_rwlock_read(void* pRwLock) {
    pthread_rwlock_rdlock((pthread_rwlock_t*)pRwLock);
    return NV_OK;
}

NV_STATUS os_acquire_rwlock_write(void* pRwLock) {
    pthread_rwlock_wrlock((pthread_rwlock_t*)pRwLock);
    return NV_OK;
}

NV_STATUS os_cond_acquire_rwlock_read(void* pRwLock) {
    int result = pthread_rwlock_tryrdlock((pthread_rwlock_t *)pRwLock);
    if (result == 0) {
        return NV_OK;
    } else {
        return NV_ERR_TIMEOUT_RETRY;
    }

}

NV_STATUS os_cond_acquire_rwlock_write(void* pRwLock) {
    int result = pthread_rwlock_tryrdlock((pthread_rwlock_t *)pRwLock);
    if (result == 0) {
        return NV_OK;
    } else {
        return NV_ERR_TIMEOUT_RETRY;
    }

}

void os_release_rwlock_read(void* pRwLock) {
    pthread_rwlock_unlock((pthread_rwlock_t*)pRwLock);
}

void os_release_rwlock_write(void* pRwLock) {
    pthread_rwlock_unlock((pthread_rwlock_t*)pRwLock);
}
