//
//  NVMacRMService.cpp
//  NVMac
//
//  Created by Spotlight Deveaux on 2025-06-02.
//

#include "nv_darwin.h"
#include "NVMacRMService.hpp"

OSDefineMetaClassAndStructors(NVMacRMService, IOService);

/// Global state for the control device.
nv_darwin_state_t nvd_state;

bool NVMacRMService::init(OSDictionary* dictionary) {
    bool superResult = IOService::init(dictionary);
    if (!superResult) {
        nvd_log("Initialization failed");
        return false;
    }

    nvd_state = IONewZero(nv_darwin_state_s, 1);
    if (!nvd_state) {
        return false;
    }

    return true;
}

void NVMacRMService::free(void) {
    IOPCIDevice* globalDevice = NV_GLOBAL_DEVICE;
    OSSafeReleaseNULL(globalDevice);
    IOSafeDeleteNULL(nvd_state, nv_darwin_state_s, 1);
    IOService::free();
}

IOService* NVMacRMService::probe(IOService* provider, SInt32* score) {
    if (IOService::probe(provider, score) == 0)
        return NULL;
    
    return this;
}

bool NVMacRMService::start(IOService* provider) {
    kern_return_t ret = IOService::start(provider);
    if (ret != KERN_SUCCESS) {
        nvd_log("Error while starting service (return code %d)", ret);
        Stop(provider);
        return ret;
    }
    nvd_state->service = (void*)this;
    RegisterService();

    // We will never have a seperate stack.
    nvidia_stack_t* sp = NULL;
    rm_init_rm(sp);

    // We've now started initializing the NVIDIA RM.
    // Initialize ourselves - we are the device.
    //
    // We should have an IOPCIDevice.
    IOPCIDevice* potentialDevice = OSDynamicCast(IOPCIDevice, provider);
    if (potentialDevice == NULL) {
        nvd_log("Provider is not IOPCIDevice?");
        Stop(provider);
        return kIOReturnNoDevice;
    }

    // XXX: This is somewhat what occurs with `nv_pci_probe`
    // in the Linux kernel module source.
    //
    // We only support one PCI device for right now.
    ret = potentialDevice->open(this, 0);
    if (ret != kIOReturnSuccess) {
        nvd_log("Error while opening PCI device (return code %d)", ret);
        Stop(provider);
        return kIOReturnNoDevice;
    }
    potentialDevice->setMemoryEnable(true);
    potentialDevice->setBusLeadEnable(true);

    // Look for a compatible BAR to interact with the GPU.
    for (uint8_t currentBAR = 0; currentBAR < NV_GPU_NUM_BARS; currentBAR++) {
        IODeviceMemory* barInfo = potentialDevice->getDeviceMemoryWithIndex(currentBAR);
        IOByteCount barSize = barInfo->getLength();
        nvd_log("For BAR %d, size is %llu", currentBAR, barSize);

        // Make sure this BAR's size is valid.
        if (barSize == 0) {
            continue;
        }

        // It seems NVIDIA takes the first valid BAR...?
        // TODO(spotlightishere): Is this correct?
        break;
    }

    // Inform the RM of our OS-specific state.
    nv_state_t* nv = NV_STATE_PTR;

    // We'll use the `NV_GLOBAL_DEVICE` macro to access this going forward.
    nvd_state->device = (void*)potentialDevice;
    NV_GLOBAL_DEVICE->retain();

    // TODO(spotlightishere): Do we really need to do this?
    // We already specify PCI matches within our kext/dext info...
    nv->pci_info.vendor_id = potentialDevice->configRead16(kIOPCIConfigurationOffsetVendorID);
    nv->pci_info.device_id = potentialDevice->configRead16(kIOPCIConfigurationOffsetDeviceID);
    nv->subsystem_id = potentialDevice->configRead16(kIOPCIConfigurationOffsetSubSystemID);
    nv->subsystem_vendor = potentialDevice->configRead16(kIOPCIConfigurationOffsetSubSystemVendorID);

    nv->pci_info.bus = potentialDevice->getBusNumber();
    nv->pci_info.slot = potentialDevice->getDeviceNumber();
    nv->pci_info.function = potentialDevice->getFunctionNumber();
    nv->pci_info.domain = 2;
    nv->os_state = (void*)nvd_state;
    nv->flags = 0;
    nv->dma_dev = {};
    nv->handle = NV_GLOBAL_DEVICE;

    for (int currentBAR = 0, j = 0;
         currentBAR < NVRM_PCICFG_NUM_BARS && j < NV_GPU_NUM_BARS;
         currentBAR++) {
        IODeviceMemory* barInfo = potentialDevice->getDeviceMemoryWithIndex(currentBAR);
        IOByteCount barSize = barInfo->getLength();
        IOPhysicalAddress barAddress = barInfo->getPhysicalAddress();

        nv->bars[j].offset = NVRM_PCICFG_BAR_OFFSET(currentBAR);
        nv->bars[j].cpu_address = barAddress;
        nv->bars[j].size = barSize;

        nvd_log("inserting bar %x @ %08llx with size %llu",
                currentBAR, barAddress, barSize);
        j++;
    }
    nv->regs = &nv->bars[NV_GPU_BAR_INDEX_REGS];
    nv->fb = &nv->bars[NV_GPU_BAR_INDEX_FB];

    rm_init_private_state(sp, NV_STATE_PTR);
    rm_set_rm_firmware_requested(sp, nv);
    rm_notify_gpu_addition(sp, nv);

    NV_STATUS status = rm_init_adapter(sp, nv);
    if (status != NV_OK) {
        nvd_log("got an error while initializing: %d", status);
    }

    // char* result = rm_get_gpu_uuid(sp, nv);
    // nvd_log("ooh: %s", result);

    return ret;
}

void NVMacRMService::stop(IOService* provider) {
    nvd_log("Stopping service...");
    if (NV_GLOBAL_DEVICE != NULL) {
        NV_GLOBAL_DEVICE->close(this, 0);
        
        // Disable operation.
        NV_GLOBAL_DEVICE->setBusLeadEnable(false);
        NV_GLOBAL_DEVICE->setMemoryEnable(false);
        nvd_state->device = NULL;
    }

    IOService::stop(provider);
}
