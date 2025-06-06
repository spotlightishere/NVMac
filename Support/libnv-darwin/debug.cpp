//
//  debug.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"
#include <string>

extern "C" {

#pragma mark - Logging

void out_string(const char* str) {
    nvd_log("%s", str);
}

int nv_printf(NvU32 debuglevel, const char* printf_format, ...) {
    // TODO(spotlightishere): Respect debug level
    va_list args;
    va_start(args, printf_format);

    char* message;
    int length = vasprintf(&message, printf_format, args);

    nvd_log("[Level %d] %s", debuglevel, message);
    va_end(args);
    IOFree(message, length);

    return 0;
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
