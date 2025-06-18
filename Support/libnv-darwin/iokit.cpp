//
//  iokit.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"

extern "C" {

#pragma mark - IOKit Properties

// These are all effectively exposed under /proc under Linux.
// We will treat nv_cap_t as an OSDictionary when used
// for directory purposes, and an OSString otherwise.
struct nv_cap {
    OSDictionary* dummy;
};

nv_cap_t* os_nv_cap_create_file_entry(nv_cap_t* parent_cap, const char* name,
                                      int mode) {
    nvd_log("Briging cap (mode %d) to IOKit property %s", mode, name);

    nv_cap_t* entry = (nv_cap_t*)IOMalloc(sizeof(nv_cap));
    entry->dummy = OSDictionary::withCapacity(1);
    return entry;
}

nv_cap_t* os_nv_cap_create_dir_entry(nv_cap_t* parent_cap, const char* name,
                                     int mode) {
    nvd_log("Briging cap (mode %d) to IOKit property %s", mode, name);

    nv_cap_t* entry = (nv_cap_t*)IOMalloc(sizeof(nv_cap));
    entry->dummy = OSDictionary::withCapacity(1);
    return entry;
}
}
