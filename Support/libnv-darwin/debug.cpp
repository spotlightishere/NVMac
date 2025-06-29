//
//  debug.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"

extern "C" {

#pragma mark - Logging

void out_string(const char* str) {
    nvd_log("%s", str);
}

// TODO(spotlightishere): We should probably do better here
#define MAX_LOG_STRING_LENGTH 8192

int nv_printf(NvU32 debuglevel, const char* printf_format, ...) {
    // TODO(spotlightishere): Respect debug level
    va_list args;
    va_start(args, printf_format);

    char message[MAX_LOG_STRING_LENGTH] = {};
    vsnprintf(message, MAX_LOG_STRING_LENGTH, printf_format, args);

    nvd_log("[Level %d] %s", debuglevel, message);
    va_end(args);

    return 0;
}

NV_STATUS nv_log_error(nv_state_t* nv, NvU32 error_number, const char* format,
                       va_list ap) {
    char message[MAX_LOG_STRING_LENGTH] = {};
    int length = vsnprintf(message, MAX_LOG_STRING_LENGTH, format, ap);

    nvd_log("[ERROR CODE %d] %s", error_number, message);
    IOFree(message, length);
    return NV_OK;
}

void os_bug_check(NvU32 bugCode, const char* bugCodeStr) {
    nvd_log("panic: %s", bugCodeStr);
}

#pragma mark - Debugging

void os_dbg_init(void) {
    // No-op
    // However, we may want to initialize a logging level eventually
}

void os_dbg_set_level(NvU32 level) {
    // Currently a no-op
}

void os_dbg_breakpoint(void) {
    // TODO: Respect NVreg_EnableDbgBreakpoint
}
}
