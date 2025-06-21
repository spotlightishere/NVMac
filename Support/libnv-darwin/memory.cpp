//
//  mempory.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <os/nv_memory_type.h>

extern "C" {
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
}
