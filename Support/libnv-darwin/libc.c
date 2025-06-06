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

#pragma mark Synchronization - Mutex

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

#pragma mark Synchronization - R/W Lock

void* NV_API_CALL os_alloc_rwlock(void) {
    void* rwlock = IOMalloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init((pthread_rwlock_t*)rwlock, NULL);
    return rwlock;
}

void os_free_rwlock(void* pRwLock) {
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
    int result = pthread_rwlock_tryrdlock((pthread_rwlock_t*)pRwLock);
    if (result == 0) {
        return NV_OK;
    } else {
        return NV_ERR_TIMEOUT_RETRY;
    }
}

NV_STATUS os_cond_acquire_rwlock_write(void* pRwLock) {
    int result = pthread_rwlock_tryrdlock((pthread_rwlock_t*)pRwLock);
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

#pragma mark Synchronization - Sempahore

#if TARGET_OS_DRIVERKIT
// In DriverKit land, we are a process external to the kernel.
// We'll use our own `mach_task_self`.
#define semaphore_task mach_task_self
#else
// In actual kernel code, it's different - `mask_task_self`
// represents the entire kernel, instead of our own KEXT task.
// We'll want to instead use `current_task`.
#define semaphore_task current_task
#endif

void* os_alloc_semaphore(NvU32 initialValue) {
    // Important note: in DriverKit land, the returned semaphore_t
    // is a mach_port_t (or, in more expanded form, uint32_t.)
    // We could leverage the fact it fits within a void pointer;
    // please modify if needed for speed.
    //
    // In actual kernel code, this is its actual type.
    semaphore_t* semaphore = (semaphore_t*)IOMalloc(sizeof(semaphore_t));

    // We set a policy of 0, or `SYNC_POLICY_FIFO`.
    semaphore_create(semaphore_task(), semaphore, 0, initialValue);
    return (void*)semaphore;
}

void os_free_semaphore(void* pSema) {
    semaphore_destroy(semaphore_task(), (semaphore_t)pSema);

    // We allocated this, and must free it.
    IOFree(pSema, sizeof(semaphore_t));
}

NvBool os_semaphore_may_sleep(void) {
    // TODO: Implement if it should not (e.g. while handling IRQs)
    return NV_TRUE;
}

NV_STATUS os_acquire_semaphore(void* pSema) {
    semaphore_wait((semaphore_t)pSema);
    return NV_OK;
}

NV_STATUS os_cond_acquire_semaphore(void* pSema) {
    // The rest of the kernel has `semaphore_wait_noblock`
    // which calls `semaphore_wait` with the internal option
    // `SEMAPHORE_TIMEOUT_NOBLOCK`.
    // Oddly, this is unavailable to DriverKit.
    //
    // However, `semaphore_timedwait` with a timebase
    // of 0 seconds and 0 nanoseconds performs the same:
    // https://github.com/apple-oss-distributions/xnu/blob/e3723e1f17661b24996789d8afc084c0c3303b26/osfmk/kern/sync_sema.c#L883-L884
    mach_timespec_t instant_timeout = {.tv_sec = 0, .tv_nsec = 0};
    kern_return_t result =
        semaphore_timedwait((semaphore_t)pSema, instant_timeout);

    switch (result) {
    case KERN_OPERATION_TIMED_OUT:
        return NV_ERR_TIMEOUT_RETRY;
    case KERN_SUCCESS:
        return NV_OK;
    default:
        return NV_ERR_GENERIC;
    }
}

NV_STATUS os_release_semaphore(void* pSema) {
    semaphore_wait((semaphore_t)pSema);
    return NV_OK;
}
