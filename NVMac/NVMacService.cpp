//
//  NVMacService.cpp
//  NVMac
//
//  Created by Spotlight Deveaux on 2025-06-02.
//

#include <IOKit/IOLib.h>

#include "NVMacService.hpp"

OSDefineMetaClassAndStructors(NVMacService, IOService);

bool NVMacService::init(OSDictionary* dict) {
  return IOService::init();
}

void NVMacService::free(void) {
  IOService::free();
}

IOService* NVMacService::probe(IOService* provider, SInt32* score) {
  if (IOService::probe(provider, score) == 0)
    return NULL;

  return this;
}

bool NVMacService::start(IOService* provider) {
  if (!IOService::start(provider))
    return false;

  IOService::registerService();
  return true;
}

void NVMacService::stop(IOService* provider) {
  IOService::stop(provider);
}
