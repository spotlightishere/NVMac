//
//  libspdm_aead.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include "libspdm_polyfill.h"

void libspdm_aead_free(void* context) {
    // Stubbed
    return;
}

bool libspdm_aead_gcm_prealloc(void** context) {
    // This is -ENODEV, but errno.h does not exist with DriverKit.
    return -19;
}

bool libspdm_aead_aes_gcm_encrypt_prealloc(
    void* context, const uint8_t* key, size_t key_size, const uint8_t* iv,
    size_t iv_size, const uint8_t* a_data, size_t a_data_size,
    const uint8_t* data_in, size_t data_in_size, uint8_t* tag_out,
    size_t tag_size, uint8_t* data_out, size_t* data_out_size) {
    // Stubbed
    return false;
}

bool libspdm_aead_aes_gcm_decrypt_prealloc(
    void* context, const uint8_t* key, size_t key_size, const uint8_t* iv,
    size_t iv_size, const uint8_t* a_data, size_t a_data_size,
    const uint8_t* data_in, size_t data_in_size, const uint8_t* tag,
    size_t tag_size, uint8_t* data_out, size_t* data_out_size) {
    // Stubbed
    return false;
}
