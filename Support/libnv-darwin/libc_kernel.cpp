//  libc_kernel.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"

extern "C" {

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
}
