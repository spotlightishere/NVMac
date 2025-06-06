//
//  power.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-06.
//

#include "nv_darwin.h"
#include <DriverKit/IOLib.h>
#include <string>

extern "C" {

#pragma mark - Power Management

// We currently do not respect power management.

void nv_idle_holdoff(nv_state_t* nv) {
    return;
}

NvBool nv_dynamic_power_available(nv_state_t* nv) {
    return NV_FALSE;
}

void nv_audio_dynamic_power(nv_state_t* nv) {
    return;
}

void nv_allow_runtime_suspend(nv_state_t* nv) {
    return;
}

void nv_disallow_runtime_suspend(nv_state_t* nv) {
    return;
}

#pragma mark - Tegra Clock

// This matches behavior within the upstream Linux driver.

NV_STATUS nv_enable_clk(nv_state_t* nv, TEGRASOC_WHICH_CLK whichClkOS) {
    return NV_ERR_NOT_SUPPORTED;
}

void nv_disable_clk(nv_state_t* nv, TEGRASOC_WHICH_CLK whichClkOS) {
    return;
}

NV_STATUS nv_get_max_freq(nv_state_t* nv, TEGRASOC_WHICH_CLK whichClkOS,
                          NvU32* pMaxFreqKHz) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_set_freq(nv_state_t* nv, TEGRASOC_WHICH_CLK whichClkOS,
                      NvU32 freqKHz) {
    return NV_ERR_NOT_SUPPORTED;
}
}
