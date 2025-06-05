//
//  libspdm_aead_aes_gcm.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stdbool.h>
#include <stdint.h>

bool libspdm_aead_aes_gcm_encrypt(const uint8_t* key, size_t key_size,
                                  const uint8_t* iv, size_t iv_size,
                                  const uint8_t* a_data, size_t a_data_size,
                                  const uint8_t* data_in, size_t data_in_size,
                                  uint8_t* tag_out, size_t tag_size,
                                  uint8_t* data_out, size_t* data_out_size) {
    // Stubbed out
    return false;
}

bool libspdm_aead_aes_gcm_decrypt(const uint8_t* key, size_t key_size,
                                  const uint8_t* iv, size_t iv_size,
                                  const uint8_t* a_data, size_t a_data_size,
                                  const uint8_t* data_in, size_t data_in_size,
                                  const uint8_t* tag, size_t tag_size,
                                  uint8_t* data_out, size_t* data_out_size) {
    // Stubbed out
    return false;
}
