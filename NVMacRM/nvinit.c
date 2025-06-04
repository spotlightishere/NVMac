//
//  nvinit.c
//  NVMacRM
//
//  Created by Spotlight Deveaux on 2025-06-03.
//

#include <nv-darwin.h>
#include <nvinit.h>

void nv_darwin_init(void) {
    // We will never have a seperate stack.
    nvidia_stack_t* sp = NULL;
    nv_state_t* nv = NULL;
    rm_init_rm(sp);
}
