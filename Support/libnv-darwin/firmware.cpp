//
//  firmware.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"
#include <string>
#include <DriverKit/OSBundle.h>

extern "C" {

const void* nv_get_firmware(nv_state_t* nv, nv_firmware_type_t fw_type,
                            nv_firmware_chip_family_t fw_chip_family,
                            const void** fw_buf, NvU32* fw_size) {
    const char* firmwarePath = nv_firmware_for_chip_family(fw_type, fw_chip_family);
    
    // Load the firmware from our KEXT/DEXT's bundle.
    OSBundle* currentBundle = OSBundle::mainBundle();
    OSMappedFile* outputFile = NULL;
    kern_return_t result = currentBundle->loadResource(firmwarePath, 0, 0, &outputFile);
    if (result != KERN_SUCCESS) {
        OSSafeReleaseNULL(currentBundle);
        return NULL;
    }
    OSSafeReleaseNULL(currentBundle);
    
    *fw_size = (NvU32)outputFile->size();
    *fw_buf = outputFile->data();
    
    return outputFile;
}


void nv_put_firmware(const void* fw_handle) {
    // We're given an OSMappedFile.
    OSMappedFile* outputFile = (OSMappedFile*)fw_handle;
    outputFile->release();
}

}
