//
//  mempory.cpp
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

#pragma mark - Helper Functions

/// Convers NVIDIA's cache modes to `kIOMemoryMapCacheMode`s as possible.
inline uint64_t nvCacheToIOOption(uint64_t nvCacheMode) {
    switch (nvCacheMode) {
    case NV_MEMORY_CACHED:
        return kIOMemoryMapCacheModeDefault;
    case NV_MEMORY_WRITECOMBINED:
        // TODO: Verify if this works
        return kIOMemoryMapCacheModeWriteThrough |
               kIOMemoryMapCacheModeCopyback;
    case NV_MEMORY_WRITEBACK:
        return kIOMemoryMapCacheModeWriteThrough;
    case NV_MEMORY_UNCACHED:
    case NV_MEMORY_DEFAULT:
        return kIOMemoryMapCacheModeInhibit;
        break;
    default:
        // We'll assume default.
        return kIOMemoryMapCacheModeDefault;
    }
}

#pragma mark - Single page DMA access

NV_STATUS os_get_page(NvU64 address) {
    // TODO: Seemingly only used for NUMA?
    return NV_ERR_NOT_SUPPORTED;
}

NvU32 os_get_page_refcount(NvU64 address) {
    // TODO: Seemingly only used for NUMA?
    return 0;
}

NV_STATUS os_put_page(NvU64 address) {
    // TODO: Seemingly only used for NUMA?
    return NV_ERR_NOT_SUPPORTED;
}

#pragma mark - DMA

NvBool os_dma_buf_enabled = NV_FALSE;

NvBool nv_requires_dma_remap(nv_state_t* nv) {
    return NV_FALSE;
}

NV_STATUS nv_dma_map_alloc(nv_dma_device_t* dma_dev, NvU64 page_count,
                           NvU64* va_array, NvBool contig, void** priv) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_dma_unmap_alloc(nv_dma_device_t* dma_dev, NvU64 page_count,
                             NvU64* va_array, void** priv) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_dma_map_peer(nv_dma_device_t* dma_dev,
                          nv_dma_device_t* peer_dma_dev, NvU8 bar_index,
                          NvU64 page_count, NvU64* va) {
    return NV_ERR_NOT_SUPPORTED;
}

void nv_dma_unmap_peer(nv_dma_device_t* dma_dev, NvU64 page_count, NvU64 va) {
    // Stubbed
}

NV_STATUS nv_dma_map_mmio(nv_dma_device_t* dma_dev, NvU64 page_count,
                          NvU64* va) {
    return NV_ERR_NOT_SUPPORTED;
}

void nv_dma_unmap_mmio(nv_dma_device_t* dma_dev, NvU64 page_count, NvU64 va) {
    // Stubbed
}

void nv_dma_cache_invalidate(nv_dma_device_t* dma_dev, void* priv) {
    // Stubbed
}

NV_STATUS nv_dma_import_sgt(nv_dma_device_t* dma_dev, struct sg_table* sgt,
                            struct drm_gem_object* gem) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_dma_import_dma_buf(nv_dma_device_t* dma_dev,
                                struct dma_buf* dma_buf, NvU32* size,
                                struct sg_table** sgt,
                                nv_dma_buf_t** import_priv) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_dma_import_from_fd(nv_dma_device_t* dma_dev, NvS32 fd, NvU32* size,
                                struct sg_table** sgt,
                                nv_dma_buf_t** import_priv) {
    return NV_ERR_NOT_SUPPORTED;
}

void nv_dma_release_sgt(struct sg_table* sgt, struct drm_gem_object* gem) {
    // Stubbed
}

void nv_dma_release_dma_buf(nv_dma_buf_t* import_priv) {
    // Stubbed
}

void nv_set_dma_address_size(nv_state_t* nv, NvU32 phys_addr_bits) {
    // Persist bits.
    nvd_log("Persisting %d bits for DMA address.", phys_addr_bits);
    uint64_t dma_mask = (((uint64_t)1) << phys_addr_bits) - 1;

    nvd_state->dma_mask = dma_mask;
    return;
}

#pragma mark - Kernel Mapping

#ifdef TARGET_OS_DRIVERKIT
NvU32 os_page_size = (NvU32)PAGE_SIZE;
NvU64 os_page_mask = (NvU64)PAGE_MASK;
// e.g. 1 << 12 is 4096, 1 << 14 is 16384.
NvU8 os_page_shift = (NvU8)PAGE_SHIFT;
#else
NvU32 os_page_size = (NvU32)IOVMPageSize;
NvU64 os_page_mask = ~(os_page_size - 1);
// e.g. 1 << 12 is 4096, 1 << 14 is 16384.
NvU8 os_page_shift = std::countr_zero(os_page_size);
#endif

void* os_map_kernel_space(NvU64 start, NvU64 size_bytes, NvU32 mode) {
    nvd_log("[libnv-darwin] Mapping %p to size %llu", (void*)start, size_bytes);

    // TODO: We assume only one device, and one state pointer.
    // Adjust as necessary.
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
        nvd_log("Requested to map %llu with size of %llu.", start, size_bytes);
        return NULL;
    }

    // Perfect - we have a valid BAR index.
    // Get a memory descriptor so that we can map it within our process.
    IOMemoryDescriptor* returnMemory = NULL;
    kern_return_t result = device->_CopyDeviceMemoryWithIndex(
        memoryIndex, &returnMemory, ((IOService*)nvd_state->service));
    if (result != KERN_SUCCESS) {
        nvd_log("Failed to map memory: %d", result);
        return NULL;
    }

    // Determine a caching option.
    uint64_t mappingOption = nvCacheToIOOption(mode);

    // Map this range into our space.
    IOMemoryMap* mapping = NULL;
    result = returnMemory->CreateMapping(mappingOption, 0, 0, size_bytes, 0,
                                         &mapping);
    if (result != KERN_SUCCESS) {
        nvd_log("Failed to create memory mapping: %d", result);
        OSSafeReleaseNULL(returnMemory);
        return NULL;
    }

    // TODO(spotlightishere): Determine a better way
    // to manage mappings such as this.
    void* addressBase = (void*)mapping->GetAddress();

    // We expect `%p` to produce format `0xabcdef0123456789`.
    // This should be twice its size (e.g. 8 bytes -> 16 characters),
    // and have the suffix `0x` at the beginning.
    constexpr auto keyNameLength = (sizeof(void*) * 2) + 2;
    char keyName[keyNameLength] = {};
    snprintf(keyName, keyNameLength, "%p", addressBase);

    // TODO(spotlightishere): =(
    global_mapped_memory->setObject(keyName, mapping);

    OSSafeReleaseNULL(returnMemory);
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

    // Translate options.
    // We are not handling encrypted pages, or node IDs.
    //
    // There appears to be a way to encrypt pages within IOKit,
    // but not from APIs present in DriverKit...
    uint64_t options = kIOMemoryDirectionInOut;
    // TODO(spotlightishere): Remove, eventually
    if (totalSize >= (1 * 1024 * 1024 * 1024)) {
        totalSize = page_count * 1 * 1024 * 1024 * 1024;
        //        __builtin_debugtrap();
    }

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

    nvd_log("Returned a user mapping for %llu", pageAddress);
    return NV_OK;
}

NV_STATUS nv_free_user_mapping(nv_state_t* nv, void* pAllocPrivate,
                               NvU64 userAddress, void* pPrivate) {
    // TODO: No-op?
    return NV_OK;
}
}
