//
//  nv-darwin.h
//  NVMac
//
//  Created by Spotlight Deveaux on 2025-06-03.
//

#ifndef nv_darwin_h
#define nv_darwin_h

#include <nv.h>

typedef struct nv_darwin_state_s {
    nv_state_t nv_state;
} nv_darwin_state_t;

#define NV_STATE_PTR(nvd) &(((nv_darwin_state_t*)(nvd))->nv_state)

#endif /* nv_darwin_h */
