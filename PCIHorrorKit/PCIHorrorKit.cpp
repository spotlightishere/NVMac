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
nv_darwin_state_t nvd_state;

bool PCIHorrorKit::init() {
    bool ret = super::init();
    if (!ret) {
        nvd_log("Initialization failed");
        return false;
    }

    nvd_state = IONewZero(nv_darwin_state_s, 1);
    if (!nvd_state) {
        return false;
    }

    return true;
}

void PCIHorrorKit::free() {
    IOSafeDeleteNULL(nvd_state, nv_darwin_state_s, 1);
    super::free();
}

kern_return_t PCIHorrorKit::Start_Impl(IOService* provider) {
    // Console logs appear to fail without this.
    // TODO(spotlightishere): Eventually clean this up...
    IOSleep(500);

    kern_return_t ret = Start(provider, SUPERDISPATCH);
    if (ret != KERN_SUCCESS) {
        nvd_log("Error while starting service (return code %d)", ret);
        return ret;
    }
    
    // We should have an IOPCIDevice.
    IOPCIDevice* device = OSDynamicCast(IOPCIDevice, provider);
    if (device == NULL) {
        nvd_log("Provider is not IOPCIDevice?");
        nvd_log("Instead, this provider is present: %s", provider->GetClassName());
        Stop(provider);
        return kIOReturnNoDevice;
    }
    // Use the `NV_GLOBAL_DEVICE` macro to access this.
    nvd_state->device = (void*)device;
    
    // Here goes nothing...
    nvd_log("Hello, world!");
    // We will never have a seperate stack.
    nvidia_stack_t* sp = NULL;
    rm_init_rm(sp);
    
    // Inform the RM of our OS-specific state.
    nv_state_t *nv = NV_STATE_PTR;
    nv->os_state = (void*)nvd_state;
    
    char* result = rm_get_gpu_uuid(sp, nv);
    nvd_log("ooh: %s", result);
    
//    NvBool status = rm_is_supported_device(sp, nv);
//    nvd_log("supported device? got %d", status);
    
    nvd_log("hmm: %s", provider->GetClassName());
    
    return ret;
}

kern_return_t PCIHorrorKit::Stop_Impl(IOService* provider) {
    nvd_log("Stopping service...");
    return Stop(provider, SUPERDISPATCH);
}
