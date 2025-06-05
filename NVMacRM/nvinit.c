//
//  nvinit.c
//  NVMacRM
//
//  Created by Spotlight Deveaux on 2025-06-03.
//

#include <nv-darwin.h>
#include <nvinit.h>

// Global state for the control device
nv_darwin_state_t nv_ctl_device = { { 0 } };

void nv_darwin_init(void) {
    // We will never have a seperate stack.
    nvidia_stack_t* sp = NULL;
    rm_init_rm(sp);
    
    // Inform the RM of our OS-specific state.
    nv_state_t *nv = NV_STATE_PTR(&nv_ctl_device);
    nv->os_state = (void*)&nv_ctl_device;
    
//    char* result = rm_get_gpu_uuid(sp, nv);
}
