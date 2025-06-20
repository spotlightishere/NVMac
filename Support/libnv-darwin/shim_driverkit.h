//
//  shim_driverkit.h
//  libnv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#ifndef shim_driverkit_h
#define shim_driverkit_h

#pragma mark - Imports

// All IOKit types are exported within DriverKit.framework.
#include <DriverKit/IOBufferMemoryDescriptor.h>
#include <DriverKit/IOCommandPool.h>
#include <DriverKit/IODMACommand.h>
#include <DriverKit/IOLib.h>
#include <DriverKit/IOMemoryMap.h>

// IOPCIDevice
#include <PCIDriverKit/PCIDriverKit.h>

// Similarly, all OS types are as well.
#include <DriverKit/OSDictionary.h>
#include <DriverKit/OSNumber.h>
#include <DriverKit/OSString.h>

// Necessary for `mach_port_t`.
#include <DriverKit/IORPC.h>

// Various I/O functions
#include <stdio.h>

// `clock_gettime_nsec_np`
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark - Mach types

// https://github.com/apple-oss-distributions/xnu/blob/e3723e1f17661b24996789d8afc084c0c3303b26/osfmk/mach/clock_types.h#L56-L65
typedef int clock_res_t;
struct mach_timespec {
    unsigned int tv_sec; /* seconds */
    clock_res_t tv_nsec; /* nanoseconds */
};
typedef struct mach_timespec mach_timespec_t;

// We'll need access to our own Mach port for semaphores.
typedef mach_port_t task_t;
task_t mach_task_self(void);

// In DriverKit land, we are a process external to the kernel.
// We'll use our own `mach_task_self` for `nvd_task`.
#define nvd_task mach_task_self

kern_return_t pid_for_task(task_t task, int32_t* pid);
// proc_name_t is 2*MAXCOMLEN+1 bytes, and must remain
// at this size in order to respect the existing ABI.
// https://github.com/apple-oss-distributions/xnu/blob/e3723e1f17661b24996789d8afc084c0c3303b26/osfmk/mach/resource_monitors.h#L66-L77
#define MAXCOMLEN 16
typedef char proc_name_t[2 * MAXCOMLEN + 1];

void proc_name(int pid, char* buf, int size);

typedef uint32_t uid_t;
uid_t geteuid(void);

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

#pragma mark - Semaphore

// A `semaphore_t` is an `mach_port_t` within userspace:
// https://github.com/apple-oss-distributions/xnu/blob/e3723e1f17661b24996789d8afc084c0c3303b26/osfmk/mach/mach_types.h#L172-L175
// DriverKit is in an odd position where it does get a Mach port.
//
// A `mach_port_t` is an `uint32_t`.
// As we (ab)use casting between the two with semaphores,
// let us instead define semaphore_t as a `uint64_t`.
// (This avoids the compiler giving a warning.)
typedef uint64_t semaphore_t;

// Used for `semaphore_timedwait`.
// https://github.com/apple-oss-distributions/xnu/blob/e3723e1f17661b24996789d8afc084c0c3303b26/osfmk/mach/kern_return.h#L309
#define KERN_OPERATION_TIMED_OUT 49

kern_return_t semaphore_create(task_t task, semaphore_t* semaphore, int policy,
                               int value);
kern_return_t semaphore_wait(semaphore_t semaphore);
kern_return_t semaphore_timedwait(semaphore_t semaphore,
                                  mach_timespec_t wait_time);
kern_return_t semaphore_signal(semaphore_t semaphore);
kern_return_t semaphore_destroy(task_t task, semaphore_t semaphore);

#ifdef __cplusplus
}
#endif

#endif /* driverkit_shim_h */
