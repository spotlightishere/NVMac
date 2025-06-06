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

/// Definition of `os_unfair_lock_s` for DriverKit.
/// https://github.com/apple-oss-distributions/libplatform/blob/9ca0fd4b4be3ee51eef43664ef75946188dfd343/include/os/lock.h#L86-L89
// TODO(spotlightishere): There has to be a better way to do this...
typedef struct os_unfair_lock_s {
    uint32_t _os_unfair_lock_opaque;
} os_unfair_lock, *os_unfair_lock_t;

void os_unfair_lock_lock(os_unfair_lock_t lock);
void os_unfair_lock_unlock(os_unfair_lock_t lock);

#ifdef __cplusplus
}
#endif

#endif /* driverkit_shim_h */
