//
//  driverkit_shim.h
//  NVMac
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#ifndef driverkit_shim_h
#define driverkit_shim_h

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark - os_unfair_lock

/// Definition of `os_unfair_lock_s` for DriverKit.
/// https://github.com/apple-oss-distributions/libplatform/blob/9ca0fd4b4be3ee51eef43664ef75946188dfd343/include/os/lock.h#L86-L89
// TODO(spotlightishere): There has to be a better way to do this...
typedef struct os_unfair_lock_s {
    uint32_t _os_unfair_lock_opaque;
} os_unfair_lock, *os_unfair_lock_t;

void os_unfair_lock_lock(os_unfair_lock_t lock);
void os_unfair_lock_unlock(os_unfair_lock_t lock);

#pragma mark - pthread mutex
typedef os_unfair_lock _pthread_lock;

typedef struct _pthread_mutex_ulock_s {
    uint32_t uval;
}* _pthread_mutex_ulock_t;

// Edited to assume __LP64__ since we really need size.
typedef struct pthread_mutex_s {
    long sig;
    _pthread_lock lock;
    uint32_t value;
    int16_t prioceiling;
    int16_t priority;
    uint32_t _pad;
    uint32_t m_tid[2];
    uint32_t m_seq[2];
    uint32_t m_mis[2];
    uint32_t _reserved[4];
} pthread_mutex_t;

int pthread_mutex_init(pthread_mutex_t* mutex, void* attr);
int pthread_mutex_destroy(pthread_mutex_t* mutex);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_trylock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);

#pragma mark - pthread rw lock

typedef struct pthread_rwlock_s {
    long sig;
    _pthread_lock lock;
    uint32_t unused : 29, misalign : 1, pshared : 2;
    uint32_t rw_flags;
    uint32_t _pad;
    uint32_t rw_tid[2];
    uint32_t rw_seq[4];
    uint32_t rw_mis[4];
    uint32_t _reserved[34];
} pthread_rwlock_t;

int pthread_rwlock_init(pthread_rwlock_t* rwlock, void* attr);
int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t* rwlock);

#ifdef __cplusplus
}
#endif

#endif /* driverkit_shim_h */
