//
//  acpi.cpp
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#include "nv_darwin.h"

extern "C" {

// Let's avoid ACPI for now.

NV_STATUS os_get_acpi_rsdp_from_uefi(NvU32* pRsdpAddr) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_acpi_d3cold_dsm_for_upstream_port(nv_state_t* nv,
                                               NvU8* pAcpiDsmGuid,
                                               NvU32 acpiDsmRev,
                                               NvU32 acpiDsmSubFunction,
                                               NvU32* data) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_acpi_ddc_method(nv_state_t* nv, void* pEdidBuffer, NvU32* pSize,
                             NvBool bReadMultiBlock) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS NV_API_CALL nv_acpi_dod_method(nv_state_t* nv, NvU32* pOutData,
                                         NvU32* pSize) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS nv_acpi_dsm_method(nv_state_t* nv, NvU8* pAcpiDsmGuid,
                             NvU32 acpiDsmRev, NvBool acpiNvpcfDsmFunction,
                             NvU32 acpiDsmSubFunction, void* pInParams,
                             NvU16 inParamSize, NvU32* outStatus,
                             void* pOutData, NvU16* pSize) {
    return NV_ERR_NOT_SUPPORTED;
}

NvBool NV_API_CALL nv_acpi_is_battery_present(void) {
    return NV_FALSE;
}

void NV_API_CALL nv_acpi_methods_init(NvU32* handlesPresent) {
    return;
}

void NV_API_CALL nv_acpi_methods_uninit(void) {
    return;
}

NV_STATUS NV_API_CALL nv_acpi_mux_method(nv_state_t* nv, NvU32* pInOut,
                                         NvU32 muxAcpiId,
                                         const char* pMethodName) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS NV_API_CALL nv_acpi_rom_method(nv_state_t* nv, NvU32* pInData,
                                         NvU32* pOutData) {
    return NV_ERR_NOT_SUPPORTED;
}

NV_STATUS NV_API_CALL nv_acpi_get_powersource(NvU32* ac_plugged) {
    return NV_ERR_NOT_SUPPORTED;
}

NvBool nv_platform_supports_s0ix(void) {
    return NV_FALSE;
}
}
