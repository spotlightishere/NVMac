//
//  libc.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"

// TODO(spotlightishere): There's no way this will scale
static OSDictionary* global_allocations = OSDictionary::withCapacity(16);

extern "C" {

#pragma mark - Memory Management

NV_STATUS os_alloc_mem(void** address, NvU64 size) {
    void* ptr = IOMallocZero(size);
    if (ptr == NULL) {
        return NV_ERR_NO_MEMORY;
    }

    // TODO(spotlightishere): I apologize.
    // We expect `%p` to produce format `0xabcdef0123456789`.
    // This should be twice its size (e.g. 8 bytes -> 16 characters),
    // and have the suffix `0x` at the beginning.
    constexpr auto keyNameLength = (sizeof(void*) * 2) + 2;
    char keyName[keyNameLength] = {};
    snprintf(keyName, keyNameLength, "%p", ptr);

    // Our key is our pointer in string form.
    OSString* key = OSString::withCString(keyName);
    // Our value is our size as an OSNumber.
    OSNumber* value = OSNumber::withNumber(size, sizeof(NvU64));

    // TODO(spotlightishere): =(
    global_allocations->setObject(key, value);
    *address = ptr;

    OSSafeReleaseNULL(key);
    OSSafeReleaseNULL(value);
    return NV_OK;
}

void os_free_mem(void* ptr) {
    // TODO(spotlightishere): Please refer to above :(
    constexpr auto keyNameLength = (sizeof(void*) * 2) + 2;
    char keyName[keyNameLength] = {};
    snprintf(keyName, keyNameLength, "%p", ptr);

    // Our key is our pointer in string form.
    OSString* key = OSString::withCString(keyName);
    OSObject* object = global_allocations->getObject(key);
    if (object == NULL) {
        nvd_log("TODO Pointer was freed for allocation that does not exist!");

        OSSafeReleaseNULL(key);
        return;
    }

    OSNumber* allocationValue = OSRequiredCast(OSNumber, object);
    uint64_t allocationSize = allocationValue->unsigned64BitValue();
    IOFree(ptr, allocationSize);

    global_allocations->removeObject(key);
    OSSafeReleaseNULL(key);
}

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
    void* spinlock = IOSimpleLockAlloc();
    *ppSpinlock = spinlock;

    if (spinlock == NULL) {
        return NV_ERR_NO_MEMORY;
    } else {
        return NV_OK;
    }
}

void os_free_spinlock(void* pSpinlock) {
    IOSimpleLockFree((IOSimpleLock*)pSpinlock);
}

// Return value are flags that we do not respect.
NvU64 os_acquire_spinlock(void* pSpinlock) {
    IOSimpleLockLock((IOSimpleLock*)pSpinlock);
    return 0;
}

void os_release_spinlock(void* pSpinlock, NvU64 oldIrql) {
    IOSimpleLockUnlock((IOSimpleLock*)pSpinlock);
}

#pragma mark Synchronization - Mutex

NV_STATUS os_alloc_mutex(void** ppMutex) {
    IOLock* lock = IOLockAlloc();
    if (lock == NULL) {
        return NV_ERR_NO_MEMORY;
    } else {
        return NV_OK;
    }
}

void os_free_mutex(void* pMutex) {
    IOLockFree((IOLock*)pMutex);
}

NV_STATUS os_acquire_mutex(void* pMutex) {
    IOLockLock((IOLock*)pMutex);
    return NV_OK;
}

NV_STATUS os_cond_acquire_mutex(void* pMutex) {
    bool result = IOLockTryLock((IOLock*)pMutex);
    if (result == 0) {
        return NV_OK;
    } else {
        return NV_ERR_TIMEOUT_RETRY;
    }
}

void os_release_mutex(void* pMutex) {
    IOLockUnlock((IOLock *)pMutex);
}

#pragma mark Synchronization - R/W Lock

void* os_alloc_rwlock(void) {
    return IORWLockAlloc();
}

void os_free_rwlock(void* pRwLock) {
    IORWLockFree((IORWLock *)pRwLock);
}

NV_STATUS os_acquire_rwlock_read(void* pRwLock) {
    IORWLockRead((IORWLock *)pRwLock);
    return NV_OK;
}

NV_STATUS os_acquire_rwlock_write(void* pRwLock) {
    IORWLockWrite((IORWLock *)pRwLock);
    return NV_OK;
}

NV_STATUS os_cond_acquire_rwlock_read(void* pRwLock) {
    IORWLockTryRead((IORWLock *)pRwLock);
    return NV_OK;
}

NV_STATUS os_cond_acquire_rwlock_write(void* pRwLock) {
    IORWLockTryWrite((IORWLock *)pRwLock);
    return NV_OK;
}

void os_release_rwlock_read(void* pRwLock) {
    IORWLockUnlock((IORWLock *)pRwLock);
}

void os_release_rwlock_write(void* pRwLock) {
    IORWLockUnlock((IORWLock *)pRwLock);
}

#pragma mark Synchronization - Sempahore

void* os_alloc_semaphore(NvU32 initialValue) {
    // Important note: in DriverKit land, the returned semaphore_t
    // is a mach_port_t (or, in more expanded form, uint32_t.)
    // We could leverage the fact it fits within a void pointer;
    // please modify if needed for speed.
    //
    // In actual kernel code, this is its actual type.
    semaphore_t* semaphore = (semaphore_t*)IOMalloc(sizeof(semaphore_t));

    // We set a policy of 0, or `SYNC_POLICY_FIFO`.
    semaphore_create(nvd_task(), semaphore, 0, initialValue);
    return (void*)semaphore;
}

void os_free_semaphore(void* pSema) {
    semaphore_destroy(nvd_task(), (semaphore_t)pSema);

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
#if TARGET_OS_DRIVERKIT
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
#else
    kern_return_t result = semaphore_wait_noblock((semaphore_t)pSema);
#endif
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
}
