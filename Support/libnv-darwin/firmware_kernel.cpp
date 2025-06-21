//
//  firmware_kernel.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"

// Weirdly, `vnode_verify_kind_t` is not
// defined anywhere as of macOS 26.0.
//
// It's required to import <sys/vnode.h>.
typedef uint32_t vnode_verify_kind_t;

#include <sys/vnode.h>

extern "C" {

// TODO(spotlightishere): Using the `#embed`
// directive seems to have clang take up ~6 gb
// of memory, alongside taking 3 minutes to compile.
//
// We should eventually determine KEXT location,
// or a better way to embed firmware.
//
// In the meantime, as a workaround, we make
// Xcode define its `PROJECT_DIR` as a macro.
#define NVIDIA_FIRMWARE_DIR XCODE_PROJECT_DIR "/Support/nvidia-binary-driver"

struct nv_darwin_fw_context {
    void* buffer;
    size_t buffer_size;
};

const void* nv_get_firmware(nv_state_t* nv, nv_firmware_type_t fw_type,
                            nv_firmware_chip_family_t fw_chip_family,
                            const void** fw_buf, NvU32* fw_size) {
    // We'll use `nv_darwin_fw_context` to preserve size metadata when freeing.
    nv_darwin_fw_context* context =
        (nv_darwin_fw_context*)IOMalloc(sizeof(nv_darwin_fw_context));
    if (context == NULL) {
        return NULL;
    }

    // Determine the path to our firmware.
    // This emits a path such as `firmware/gsp_ga10x.bin`.
    const char* firmwareSubpath =
        nv_firmware_for_chip_family(fw_type, fw_chip_family);
    char firmwarePath[PATH_MAX] = NVIDIA_FIRMWARE_DIR;
    strncpy(firmwarePath, firmwareSubpath, PATH_MAX);

    // Next, open the firmware and read its contents.
    //
    // Derived from the following code within Lilu:
    // https://github.com/acidanthera/Lilu/blob/dc01cb583295ceded4f42a6310a15e3abac1c025/Lilu/Sources/kern_file.cpp
    vnode_t vnode = NULLVP;
    vfs_context_t vfs_context = vfs_context_create(NULL);

    errno_t result = vnode_lookup(firmwarePath, 0, &vnode, vfs_context);
    if (result) {
        nvd_log("Failed to open firmware! (error %d, path %s)", result,
                firmwarePath);
        vfs_context_rele(vfs_context);
        return NULL;
    }

    // Read its size, and allocate our buffer.
    vnode_attr va;
    VATTR_INIT(&va);
    VATTR_WANTED(&va, va_data_size);
    result = vnode_getattr(vnode, &va, vfs_context);
    if (result) {
        nvd_log("Failed to get file size of firmware! (error %d)", result);
        vnode_put(vnode);
        vfs_context_rele(vfs_context);
        return NULL;
    }
    context->buffer_size = va.va_data_size;

    context->buffer = IOMallocData(context->buffer_size);
    if (context->buffer == NULL) {
        panic("Failed to allocate firmware bufffer!");
    }

    result = vn_rdwr(
        UIO_READ, vnode, (caddr_t)context->buffer, (int)context->buffer_size, 0,
        UIO_SYSSPACE, IO_NODELOCKED | IO_NOCACHE | IO_RAOFF,
        vfs_context_ucred(vfs_context), NULL, vfs_context_proc(vfs_context));
    if (result) {
        IOLog("Failed to read firmware! (error %d)", result);
        vnode_put(vnode);
        vfs_context_rele(vfs_context);
        return NULL;
    }

    // We're all done! Release our vnode.
    vnode_put(vnode);
    vfs_context_rele(vfs_context);

    *fw_buf = context->buffer;
    *fw_size = (NvU32)context->buffer_size;
    return context;
}

void nv_put_firmware(const void* fw_handle) {
    // Free the internal buffer and context.
    nv_darwin_fw_context* context = (nv_darwin_fw_context*)fw_handle;
    if (context->buffer != NULL) {
        IOFree(context->buffer, context->buffer_size);
    }

    IOFree(context, sizeof(nv_darwin_fw_context));
}
}
