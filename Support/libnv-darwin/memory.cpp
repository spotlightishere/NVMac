//
//  mempory.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <DriverKit/IOBufferMemoryDescriptor.h>
#include <DriverKit/IOMemoryMap.h>
#include <PCIDriverKit/PCIDriverKit.h>
#include <map>
#include <os/nv_memory_type.h>

#pragma mark - Internal Mapping

/// A global map of buffer pointers to their underlying IOMemoryMapping.
// TODO(spotlightishere): There's no way this will scale
std::map<void*, IOMemoryMap*> global_mapped_memory{};

/// NVIDIA does a lot of operations per-page, where we map in bulk.
/// We deal with a single `IOMemoryMap` for `IOBufferedMemoryDescriptors`.
/// This structure exists to help us pair between them all
///
/// An alternative name for this structure would be `IOMemoryMapMapping`,
/// but that's hard to follow. We'll call it `IOMemoryMapEntry`.
struct IOMemoryMapEntry {
    /// The underlying memory descriptor.
    IOBufferMemoryDescriptor* descriptor;

    /// The underlying IOMemoryMap we currently have.
    IOMemoryMap* mapping;

    /// The amount of times we've been requested to map.
    /// Initially, we assume we have mapped once.
    ///
    /// This is used to ensure we do not unmap prematurely.
    uint32_t mappingCount;

    IOMemoryMapEntry(IOBufferMemoryDescriptor* descriptor,
                     IOMemoryMap* mapping) {
        this->descriptor = descriptor;
        this->mapping = mapping;
        this->mappingCount = 1;
    }
};

extern "C" {

#pragma mark - Helper Functions

/// Convers NVIDIA's cache modes to `kIOMemoryMapCacheMode`s as possible.
uint64_t nvCacheToIOOption(uint64_t nvCacheMode) {
    switch (nvCacheMode) {
    case NV_MEMORY_CACHED:
        nvd_log("going with kIOMemoryMapCacheModeDefault");
        return kIOMemoryMapCacheModeDefault;
    case NV_MEMORY_WRITECOMBINED:
        nvd_log("going with write combined");
        // TODO: Verify if this works
        return kIOMemoryMapCacheModeWriteThrough |
               kIOMemoryMapCacheModeCopyback;
    case NV_MEMORY_WRITEBACK:
        nvd_log("going with write-through");
        return kIOMemoryMapCacheModeWriteThrough;
    case NV_MEMORY_UNCACHED:
    case NV_MEMORY_UNCACHED_WEAK:
    case NV_MEMORY_DEFAULT:
        nvd_log("going with default inhibit");
        return kIOMemoryMapCacheModeInhibit;
        break;
    default:
        // We'll assume default.
        nvd_log("Unknown cache option specified (%llu); assuming cached.",
                nvCacheMode);
        return kIOMemoryMapCacheModeDefault;
    }
}

#pragma mark - APIs

// TODO(spotlightishere): This is hardcoded
NvU32 os_page_size = 4096;
NvU64 os_page_mask = ~(os_page_size - 1);
NvU8 os_page_shift = 12;

void* os_map_kernel_space(NvU64 start, NvU64 size_bytes, NvU32 mode) {
    // Our start contains our memoryIndex.
    uint8_t memoryIndex = (uint8_t)start & 0xFF;

    nvd_log("[libnv-darwin] Mapping at memory index %hhu with size %llu",
            memoryIndex, size_bytes);
    IOPCIDevice* device = (IOPCIDevice*)nvd_state->device;

    IOMemoryDescriptor* returnMemory = NULL;
    kern_return_t result = device->_CopyDeviceMemoryWithIndex(
        memoryIndex, &returnMemory, ((IOService*)nvd_state->service));
    if (result != KERN_SUCCESS) {
        nvd_log("Failed to map memory: %d", result);
        // TODO: This should be fatal
        return NULL;
    }

    // Determine what option to use.
    uint64_t mappingOptions = nvCacheToIOOption(mode);

    // Map this range into our space.
    IOMemoryMap* mapping = NULL;
    result = returnMemory->CreateMapping(mappingOptions, 0, 0, size_bytes, 0,
                                         &mapping);
    if (result != KERN_SUCCESS) {
        nvd_log("Failed to create memory mapping: %d", result);
        OSSafeReleaseNULL(returnMemory);
        return NULL;
    }

    // TODO(spotlightishere): Keep track of these mappings lol
    void* addressBase = (void*)mapping->GetAddress();
    global_mapped_memory[addressBase] = mapping;
    nvd_log("[libnv-darwin] mapping at %p for size %llu", addressBase,
            size_bytes);
    OSSafeReleaseNULL(returnMemory);

    return addressBase;
}

void os_unmap_kernel_space(void* addressBase, NvU64 size_bytes) {
    // No need to leverage byte size - we already have an IOMemoryMap
    // that will handle unmapping on our behalf.
    if (global_mapped_memory.contains(addressBase) == false) {
        nvd_log(
            "TEMP panic Memory was unmapped for pointer that does not exist!");
        return;
    }

    nvd_log("[libnv-darwin] unmapping %p", addressBase);

    IOMemoryMap* mapping = global_mapped_memory[addressBase];
    OSSafeReleaseNULL(mapping);
    global_mapped_memory.erase(addressBase);
}

// This would be where we support multiple devices
// (as specified by different states), but we do not.
// TODO: Implement if necessary.
NV_STATUS nv_alloc_pages(nv_state_t* nv, NvU32 page_count, NvU64 page_size,
                         NvBool contiguous, NvU32 cache_type, NvBool zeroed,
                         NvBool unencrypted, NvS32 node_id, NvU64* pte_array,
                         void** priv_data) {
    NvU64 totalSize = page_count * page_size;
    nvd_log("about to allocate %llu", totalSize);

    // Translate options.
    // We are not handling encrypted pages, or node IDs.
    //
    // There appears to be a way to encrypt pages within IOKit,
    // but not from APIs present in DriverKit...
    uint64_t options = kIOMemoryDirectionInOut;

    // We specify 0 for "no specific alignment".
    IOBufferMemoryDescriptor* descriptor;
    kern_return_t result =
        IOBufferMemoryDescriptor::Create(options, totalSize, 0, &descriptor);
    if (result != KERN_SUCCESS) {
        nvd_log("[libnv-darwin] Failed to allocate buffer: %d", result);
        return NV_ERR_NO_MEMORY;
    }

    IOAddressSegment range = {};
    descriptor->GetAddressRange(&range);
    void* ptr = (void*)range.address;
    if (ptr == NULL) {
        panic("[libnvd-darwin] IOAddressSegment address is null!");
    }

    if (zeroed) {
        // Zero ourselves.
        bzero(ptr, range.length);
    }

    // TODO(spotlightishere): We don't really have a array of page entries.
    // This might be incorrect...
    if (contiguous) {
        pte_array[0] = range.address;
    } else {
        // Manually create an array of page entries by iterating for page count.
        for (int i = 0; i < page_count; i += 1) {
            pte_array[i] = range.address + (i * page_size);
        }
    }

    // Store our descriptor as private data.
    *priv_data = (void*)descriptor;

    return NV_OK;
}

NV_STATUS nv_free_pages(nv_state_t* nv, NvU32 page_count, NvBool contiguous,
                        NvU32 cache_type, void* priv_data) {
    // We have an IOBufferMemoryDescriptor.
    IOBufferMemoryDescriptor* descriptor = (IOBufferMemoryDescriptor*)priv_data;
    OSSafeReleaseNULL(descriptor);
    return NV_OK;
}

// TODO(spotlightishere): This might be a fundamental mess...
void* nv_alloc_kernel_mapping(nv_state_t* nv, void* pAllocPrivate,
                              NvU64 pageIndex, NvU32 pageOffset, NvU64 size,
                              void** pPrivate) {
    // We should be given an IOBufferMemoryDescriptor.
    IOBufferMemoryDescriptor* descriptor =
        (IOBufferMemoryDescriptor*)pAllocPrivate;
    IOAddressSegment range = {};
    descriptor->GetAddressRange(&range);

    // Return an offset to the page specified.
    uint64_t pageAddress =
        range.address + (pageIndex * os_page_size) + pageOffset;
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
    IOAddressSegment range = {};
    descriptor->GetAddressRange(&range);

    // Return an offset to the page specified.
    uint64_t pageAddress =
        range.address + (pageIndex * os_page_size) + pageOffset;
    *pUserAddress = pageAddress;
    return NV_OK;
}

NV_STATUS nv_free_user_mapping(nv_state_t* nv, void* pAllocPrivate,
                               NvU64 userAddress, void* pPrivate) {
    // TODO: No-op?
    return NV_OK;
}
}
