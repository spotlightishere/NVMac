//
//  nvlink.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"
#include <nvlink_os.h>

extern "C" {

// Defined within `libc.cpp`.
NV_STATUS os_alloc_mem(void** address, NvU64 size);
void os_free_mem(void* address);

// Taken directly from kernel-open.
#define MAX_ERROR_STRING 512

void nvlink_print(const char* file, int line, const char* function,
                  int log_level, const char* fmt, ...) {
    va_list arglist;
    char nv_string[MAX_ERROR_STRING];
    const char* sys_log_level;

    // TODO(spotlightishere): We lack a concept of log levels.
    switch (log_level) {
    case NVLINK_DBG_LEVEL_INFO:
        sys_log_level = "[NVLINK INFO] ";
        break;
    case NVLINK_DBG_LEVEL_SETUP:
        sys_log_level = "[NVLINK DEBUG] ";
        break;
    case NVLINK_DBG_LEVEL_USERERRORS:
        sys_log_level = "[NVLINK NOTICE] ";
        break;
    case NVLINK_DBG_LEVEL_WARNINGS:
        sys_log_level = "[NVLINK WARNING] ";
        break;
    case NVLINK_DBG_LEVEL_ERRORS:
        sys_log_level = "[NVLINK ERROR] ";
        break;
    default:
        sys_log_level = "[NVLINK INFO] ";
        break;
    }

    va_start(arglist, fmt);
    vsnprintf(nv_string, sizeof(nv_string), fmt, arglist);
    va_end(arglist);

    nv_string[sizeof(nv_string) - 1] = '\0';
    nvd_log("%snvidia-nvlink: %s", sys_log_level, nv_string);
}

void* nvlink_malloc(NvLength size) {
    void* ptr;
    os_alloc_mem(&ptr, size);
    return ptr;
}

void nvlink_free(void* ptr) {
    os_free_mem(ptr);
}

void nvlink_assert(int expression) {
    // TODO(spotlightishere): Should this instead be a panic?
    nvd_log("NVLink: Assertion failed!\n");
}

void* nvlink_memcpy(void* dst, const void* src, NvLength len) {
    return memcpy(dst, src, len);
}

void* nvlink_memset(void* dest, int value, NvLength size) {
    return memset(dest, value, size);
}

void nvlink_sleep(unsigned int ms) {
    IOSleep(ms);
}

NvLength nvlink_strlen(const char* s) {
    return strlen(s);
}

NvBool os_imex_channel_is_supported = NV_FALSE;

NvS32 os_imex_channel_count(void) {
    // We do not handle IMEX channels.
    return 0;
}

NvS32 os_imex_channel_get(void) {
    // -1 is used when unsupported on Linux.
    return -1;
}
}
