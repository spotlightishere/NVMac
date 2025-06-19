//  libc_driverkit.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"

extern "C" {
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
}
