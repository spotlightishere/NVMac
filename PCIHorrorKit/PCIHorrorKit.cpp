//
//  PCIHorrorKit.cpp
//  PCIHorrorKit
//
//  Created by Spotlight Deveaux on 2025-06-03.
//

#include <os/log.h>

#include <DriverKit/DriverKit.h>
#include <DriverKit/IOLib.h>
#include <DriverKit/IOUserServer.h>
#include <PCIDriverKit/PCIDriverKit.h>

#include "PCIHorrorKit.h"
#include "nv_darwin.h"

/// Global state for the control device.
nv_darwin_state_t nv_ctl_device = { { 0 } };

kern_return_t PCIHorrorKit::Start_Impl(IOService* provider) {
    // Console logs appear to fail without this.
    // TODO(spotlightishere): Eventually clean this up...
    IOSleep(500);

    kern_return_t ret = Start(provider, SUPERDISPATCH);
    if (ret != KERN_SUCCESS) {
        nvd_log("Error while starting service (return code %d)", ret);
        return ret;
    }

    // Here goes nothing...
    nvd_log("Hello, world!");
    // We will never have a seperate stack.
    nvidia_stack_t* sp = NULL;
    rm_init_rm(sp);
    
    // Inform the RM of our OS-specific state.
    nv_state_t *nv = NV_STATE_PTR(&nv_ctl_device);
    nv->os_state = (void*)&nv_ctl_device;
    
    char* result = rm_get_gpu_uuid(sp, nv);

    return ret;
}

kern_return_t PCIHorrorKit::Stop_Impl(IOService* provider) {
    nvd_log("Stopping service...");
    return Stop(provider, SUPERDISPATCH);
}
