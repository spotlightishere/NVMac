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

kern_return_t PCIHorrorKit::Start_Impl(IOService* provider) {
    // Console logs appear to fail without this.
    // TODO(spotlightishere): Eventually clean this up...
    IOSleep(500);

    kern_return_t ret = Start(provider, SUPERDISPATCH);
    if (ret != KERN_SUCCESS) {
        nvd_log("Error while starting service (return code %d)", ret);
        return ret;
    }

    nvd_log("Hello, world!");
    return ret;
}

kern_return_t PCIHorrorKit::Stop_Impl(IOService* provider) {
    nvd_log("Stopping service...");
    return Stop(provider, SUPERDISPATCH);
}
