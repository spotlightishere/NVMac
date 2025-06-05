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

kern_return_t IMPL(PCIHorrorKit, Start) {
    kern_return_t ret;
    ret = Start(provider, SUPERDISPATCH);

    // Console logs appear to fail without this.
    // TODO(spotlightishere): Eventually clean this up...
    IOSleep(500);
    os_log(OS_LOG_DEFAULT, "PCIHorrorKit: Hello World");
    return ret;
}

kern_return_t IMPL(PCIHorrorKit, Stop) {
    os_log(OS_LOG_DEFAULT, "PCIHorrorKit: Stop");

    return Stop(provider, SUPERDISPATCH);
}
