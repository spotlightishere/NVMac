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
    IOPCIDevice* globalDevice = NV_GLOBAL_DEVICE;
    OSSafeReleaseNULL(globalDevice);
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
    nvd_state->service = (void*)this;

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
        return kIOReturnNoDevice;
    }

    // XXX: This is somewhat what occurs with `nv_pci_probe`
    // in the Linux kernel module source.
    //
    // We only support one PCI device for right now.
    ret = potentialDevice->Open(this, 0);
    if (ret != kIOReturnSuccess) {
        nvd_log("Error while opening PCI device (return code %d)", ret);
        return kIOReturnNoDevice;
    }
    uint16_t commandRegister;
    potentialDevice->ConfigurationRead16(kIOPCIConfigurationOffsetCommand,
                                         &commandRegister);
    commandRegister |= (kIOPCICommandBusMaster | kIOPCICommandMemorySpace);
    potentialDevice->ConfigurationWrite16(kIOPCIConfigurationOffsetCommand,
                                          commandRegister);

    // Look for a compatible BAR to interact with the GPU.
    for (uint8_t currentBAR = 0; currentBAR < NV_GPU_NUM_BARS; currentBAR++) {
        uint8_t memoryIndex = 0;
        uint64_t barSize = 0;
        uint8_t barType = 0;
        potentialDevice->GetBARInfo(currentBAR, &memoryIndex, &barSize,
                                    &barType);

        nvd_log("For BAR %d, size is %llu, type is %d", currentBAR, barSize,
                barType);

        // Make sure this BAR's size is valid.
        if (barSize == 0) {
            continue;
        }

        // It seems NVIDIA takes the first valid BAR...?
        // TODO(spotlightishere): Is this correct?
        break;
    }

    // Query for future use.

    // Inform the RM of our OS-specific state.
    nv_state_t* nv = NV_STATE_PTR;

    // We'll use the `NV_GLOBAL_DEVICE` macro to access this going forward.
    nvd_state->device = (void*)potentialDevice;
    NV_GLOBAL_DEVICE->retain();

    // TODO(spotlightishere): Do we really need to do this?
    // We already specify PCI matches within our kext/dext info...
    potentialDevice->ConfigurationRead16(kIOPCIConfigurationOffsetVendorID,
                                         &nv->pci_info.vendor_id);
    potentialDevice->ConfigurationRead16(kIOPCIConfigurationOffsetDeviceID,
                                         &nv->pci_info.device_id);
    potentialDevice->ConfigurationRead16(kIOPCIConfigurationOffsetSubSystemID,
                                         &nv->subsystem_id);
    potentialDevice->ConfigurationRead16(
        kIOPCIConfigurationOffsetSubSystemVendorID, &nv->subsystem_vendor);
    nv->os_state = (void*)nvd_state;
    // TODO(spotlightishere): Implement
    nv->pci_info.domain = 0;
    nv->pci_info.bus = 0;
    nv->pci_info.slot = 0;
    nv->flags = 0;
    nv->handle = NV_GLOBAL_DEVICE;

    for (int currentBAR = 0, j = 0;
         currentBAR < NVRM_PCICFG_NUM_BARS && j < NV_GPU_NUM_BARS;
         currentBAR++) {
        uint8_t memoryIndex = 0;
        uint64_t barSize = 0;
        uint8_t barType = 0;
        potentialDevice->GetBARInfo(currentBAR, &memoryIndex, &barSize,
                                    &barType);

        nv->bars[j].offset = NVRM_PCICFG_BAR_OFFSET(currentBAR);
        // We'll (mis)use this to instead store memoryIndex.
        nv->bars[j].cpu_address = memoryIndex;
        nv->bars[j].size = barSize;

        nvd_log("inserting bar %d with memoryIndex %d and size %llu",
                currentBAR, memoryIndex, barSize);
        j++;
    }
    nv->regs = &nv->bars[NV_GPU_BAR_INDEX_REGS];
    nv->fb = &nv->bars[NV_GPU_BAR_INDEX_FB];

    RegisterService();

    rm_init_private_state(sp, nv);
    rm_set_rm_firmware_requested(sp, nv);
    rm_notify_gpu_addition(sp, nv);

//    NV_STATUS status = rm_init_adapter(sp, nv);
//    if (status != NV_OK) {
//        nvd_log("got an error while initializing: %d", status);
//    }

    char* result = rm_get_gpu_uuid(sp, nv);
    nvd_log("ooh: %s", result);

    return ret;
}

kern_return_t PCIHorrorKit::Stop_Impl(IOService* provider) {
    nvd_log("Stopping service...");
    if (nvd_state->device != NULL) {
        NV_GLOBAL_DEVICE->Close(this, 0);
    }

    return Stop(provider, SUPERDISPATCH);
}
