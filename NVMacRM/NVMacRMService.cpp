//
//  NVMacRMService.cpp
//  NVMac
//
//  Created by Spotlight Deveaux on 2025-06-02.
//

#include <IOKit/IOLib.h>

#include "NVMacRMService.hpp"

OSDefineMetaClassAndStructors(NVMacRMService, IOService);

bool NVMacRMService::init(OSDictionary* dict) {
    return IOService::init();
}

void NVMacRMService::free(void) {
    IOService::free();
}

IOService* NVMacRMService::probe(IOService* provider, SInt32* score) {
    if (IOService::probe(provider, score) == 0)
        return NULL;

    return this;
}

bool NVMacRMService::start(IOService* provider) {
    if (!IOService::start(provider))
        return false;

    IOService::registerService();
    return true;
}

void NVMacRMService::stop(IOService* provider) {
    IOService::stop(provider);
}
