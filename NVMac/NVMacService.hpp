//
//  NVMacService.hpp
//  NVMac
//
//  Created by Spotlight Deveaux on 2025-06-02.
//

#ifndef NVMacService_hpp
#define NVMacService_hpp

#include <IOKit/IOService.h>

class NVMacService : public IOService {
  OSDeclareDefaultStructors(NVMacService);

public:
  virtual bool init(OSDictionary* dictionary = 0) APPLE_KEXT_OVERRIDE;
  virtual void free(void) APPLE_KEXT_OVERRIDE;
  virtual IOService* probe(IOService* provider,
                           SInt32* score) APPLE_KEXT_OVERRIDE;
  virtual bool start(IOService* provider) APPLE_KEXT_OVERRIDE;
  virtual void stop(IOService* provider) APPLE_KEXT_OVERRIDE;
};

#endif /* NVMacService_hpp */
