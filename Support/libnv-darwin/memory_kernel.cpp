//
//  memory_kernel.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <os/nv_memory_type.h>

#pragma mark - Internal Mapping

/// A global map of buffer pointers to their underlying IOMemoryMapping.
// TODO(spotlightishere): There's no way this will scale
static OSDictionary* global_mapped_memory = OSDictionary::withCapacity(16);

extern "C" {

#pragma mark - Kernel Mapping

NvU32 os_page_size = (NvU32)PAGE_SIZE;
NvU64 os_page_mask = (NvU64)PAGE_MASK;
NvU8 os_page_shift = (NvU8)PAGE_SHIFT;

void* os_map_kernel_space(NvU64 start, NvU64 size, NvU32 mode) {
    nvd_log("[libnv-darwin] Mapping %p to size %llu", (void*)start, size);
    nv_state_t* nv = NV_STATE_PTR;
    IOPCIDevice* device = NV_GLOBAL_DEVICE;

    // This function is effectively only called for mapping
    // this device's BAR addresses into accessible space.
    //
    // Ensure we have a valid address.
    uint8_t memoryIndex = 0xFF;
    for (int barIndex = 0; barIndex < NV_GPU_NUM_BARS; barIndex++) {
        nv_aperture_t currentBar = nv->bars[barIndex];
        if (currentBar.cpu_address == start) {
            memoryIndex = barIndex;
            break;
        }
    }
    if (memoryIndex == 0xFF) {
        nvd_log("Unable to find BAR for this device to use!\n");
        nvd_log("Requested to map %llu with size of %llu.", start, size);
        return NULL;
    }

    // Determine cache type.
    IOOptionBits memoryMode;
    switch (mode) {
    case NV_MEMORY_CACHED:
        memoryMode = kIOMapDefaultCache | kIOMapAnywhere;
        break;
    case NV_MEMORY_WRITECOMBINED:
        memoryMode = kIOMapWriteCombineCache | kIOMapAnywhere;
        break;
    case NV_MEMORY_UNCACHED:
    case NV_MEMORY_DEFAULT:
        memoryMode = kIOMapInhibitCache | kIOMapAnywhere;
        break;
    default:
        nvd_log("NVRM: unknown mode in os_map_kernel_space()\n");
        return NULL;
    }

    IOMemoryMap* mapping =
        device->mapDeviceMemoryWithRegister(memoryIndex, memoryMode);
    void* addressBase = (void*)mapping->getPhysicalAddress();
    // We expect `%p` to produce format `0xabcdef0123456789`.
    // This should be twice its size (e.g. 8 bytes -> 16 characters),
    // and have the suffix `0x` at the beginning.
    constexpr auto keyNameLength = (sizeof(void*) * 2) + 2;
    char keyName[keyNameLength] = {};
    snprintf(keyName, keyNameLength, "%p", addressBase);

    // TODO(spotlightishere): =(
    global_mapped_memory->setObject(keyName, mapping);

    OSSafeReleaseNULL(mapping);
    nvd_log("[libnv-darwin] Mapped to %p", addressBase);

    return addressBase;
}

void os_unmap_kernel_space(void* addressBase, NvU64 size_bytes) {
    // TODO(spotlightishere): Please refer to above :(
    constexpr auto keyNameLength = (sizeof(void*) * 2) + 2;
    char keyName[keyNameLength] = {};
    snprintf(keyName, keyNameLength, "%p", addressBase);

    // No need to leverage byte size - we already have an IOMemoryMap
    // that will handle unmapping on our behalf.
    OSObject* object = global_mapped_memory->getObject(keyName);
    IOMemoryMap* mapping = OSRequiredCast(IOMemoryMap, object);
    if (mapping == NULL) {
        nvd_log(
            "TEMP panic Memory was unmapped for pointer that does not exist!");
        return;
    }

    nvd_log("[libnv-darwin] unmapping %p", addressBase);

    global_mapped_memory->removeObject(keyName);
}

#pragma mark - DMA

/// Used to persist an `IODMACommand` and
/// `IOBufferMemoryDescriptor` for later freeing.
struct DMACommandDescriptorPairing {
    IODMACommand* command;
    IOBufferMemoryDescriptor* descriptor;
};

NV_STATUS nv_alias_pages(nv_state_t* nv, NvU32 count, NvU64 page_size,
                         NvU32 alloc_type_contiguous, NvU32 cache_type,
                         NvU64 guest_id, NvU64* pte_array, NvBool carveout,
                         void** priv_data) {
    return NV_ERR_NOT_SUPPORTED;
}

// This would be where we support multiple devices
// (as specified by different states), but we do not.
// TODO: Implement if necessary.
NV_STATUS nv_alloc_pages(nv_state_t* nv, NvU32 page_count, NvU64 page_size,
                         NvBool contiguous, NvU32 cache_type, NvBool zeroed,
                         NvBool unencrypted, NvS32 node_id, NvU64* pte_array,
                         void** priv_data) {
    NvU64 totalSize = page_count * page_size;
    const char* contigunuity = contiguous ? "contiguous" : "non-contiguous";
    nvd_log("[libnv-darwin] About to allocate pages for size %llu with count "
            "%d and %s",
            page_size, page_count, contigunuity);

    // Determine cache type.
    IOOptionBits bufferOptions = kIODirectionInOut;
    switch (cache_type) {
    case NV_MEMORY_CACHED:
        bufferOptions |= kIOMapDefaultCache;
        break;
    case NV_MEMORY_WRITECOMBINED:
        bufferOptions |= kIOMapWriteCombineCache;
        break;
    case NV_MEMORY_UNCACHED:
    case NV_MEMORY_DEFAULT:
        bufferOptions |= kIOMapInhibitCache;
        break;
    default:
        nvd_log("NVRM: unknown mode in nv_alloc_pages()\n");
        return NV_ERR_NOT_SUPPORTED;
    }

    // Map as contiguous if requested.
    if (contiguous) {
        bufferOptions |= kIOMemoryPhysicallyContiguous;
    }

    uint64_t bitMask = (1U << nvd_state->dma_mask) - 1;
    IOBufferMemoryDescriptor* descriptor =
        IOBufferMemoryDescriptor::inTaskWithPhysicalMask(
            kernel_task, bufferOptions, page_size, bitMask);
    if (descriptor == NULL) {
        nvd_log("[libnv-darwin] Failed to allocate memory for DMA!");
        return NV_ERR_NO_MEMORY;
    }
    descriptor->prepare();

    // We'll retain this IODMACommand until the pages are freed.
    IODMACommand* command = IODMACommand::withSpecification(
        kIODMACommandOutputHost64, 64, page_size, IODMACommand::kMapped,
        totalSize, 0);
    if (command == NULL) {
        nvd_log("[libnv-darwin] Failed to create DMA command!");
        descriptor->complete();
        OSSafeReleaseNULL(descriptor);
        return NV_ERR_NO_MEMORY;
    }

    // Enumerate through every created segment,
    // and register such within the PTE table.
    UInt64 offset = 0;
    IODMACommand::Segment64 segment[] = {};
    UInt32 segmentCount = 1;

    command->setMemoryDescriptor(descriptor);
    IOReturn result =
        command->gen64IOVMSegments(&offset, segment, &segmentCount);
    if (result != kIOReturnSuccess) {
        descriptor->complete();
        OSSafeReleaseNULL(descriptor);
        OSSafeReleaseNULL(command);
        return NV_ERR_NO_MEMORY;
    }

    for (int i = 0; i < segmentCount; i++) {
        pte_array[i] = segment[i].fIOVMAddr;

        if (zeroed) {
            bzero((void*)segment[i].fIOVMAddr, page_size);
        }
    }

    // Persist our descriptor and command when releasing.
    DMACommandDescriptorPairing* pairing =
        IONew(DMACommandDescriptorPairing, 1);
    pairing->command = command;
    pairing->descriptor = descriptor;
    *priv_data = pairing;

    return NV_OK;
}

NV_STATUS nv_free_pages(nv_state_t* nv, NvU32 page_count, NvBool contiguous,
                        NvU32 cache_type, void* priv_data) {
    // We have an DMACommandDescriptorPairing.
    DMACommandDescriptorPairing* pairing =
        (DMACommandDescriptorPairing*)priv_data;
    pairing->command->clearMemoryDescriptor();
    pairing->descriptor->complete();

    OSSafeReleaseNULL(pairing->command);
    OSSafeReleaseNULL(pairing->descriptor);
    return NV_OK;
}

// TODO(spotlightishere): This might be a fundamental mess...
void* nv_alloc_kernel_mapping(nv_state_t* nv, void* pAllocPrivate,
                              NvU64 pageIndex, NvU32 pageOffset, NvU64 size,
                              void** pPrivate) {
    // We should be given an IOBufferMemoryDescriptor.
    IOBufferMemoryDescriptor* descriptor =
        (IOBufferMemoryDescriptor*)pAllocPrivate;
    uint64_t baseAddress = descriptor->getPhysicalAddress();

    // Return an offset to the page specified.
    uint64_t pageAddress =
        baseAddress + (pageIndex * os_page_size) + pageOffset;
    return (void*)pageAddress;
}

NV_STATUS nv_free_kernel_mapping(nv_state_t* nv, void* pAllocPrivate,
                                 void* address, void* pPrivate) {
    // TODO: No-op?
    return NV_OK;
}

NV_STATUS nv_alloc_user_mapping(nv_state_t* nv, void* pAllocPrivate,
                                NvU64 pageIndex, NvU32 pageOffset, NvU64 size,
                                NvU32 protect, NvU64* pUserAddress,
                                void** ppPrivate) {
    // Do similar as `nv_alloc_kernel_mapping` for now.
    //
    // We should be given an IOBufferMemoryDescriptor.
    IOBufferMemoryDescriptor* descriptor =
        (IOBufferMemoryDescriptor*)pAllocPrivate;
    uint64_t baseAddress = descriptor->getPhysicalAddress();

    // Return an offset to the page specified.
    uint64_t pageAddress =
        baseAddress + (pageIndex * os_page_size) + pageOffset;
    *pUserAddress = pageAddress;

    nvd_log("Returned a user mapping for %llu", pageAddress);
    return NV_OK;
}

NV_STATUS nv_free_user_mapping(nv_state_t* nv, void* pAllocPrivate,
                               NvU64 userAddress, void* pPrivate) {
    // TODO: No-op?
    return NV_OK;
}
}
