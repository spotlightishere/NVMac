//
//  libspdm_ec.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stdbool.h>
#include <stdint.h>

bool libspdm_ec_compute_key(void* ec_context, const uint8_t* peer_public,
                            size_t peer_public_size, uint8_t* key,
                            size_t* key_size) {
    // Stubbed
    return false;
}

bool libspdm_ec_generate_key(void* ec_context, uint8_t* public_data,
                             size_t* public_size) {
    // Stubbed
    return false;
}

void libspdm_ec_free(void* ec_context) {
    // Stubbed
    return;
}

bool libspdm_ecdsa_sign(void* ec_context, size_t hash_nid,
                        const uint8_t* message_hash, size_t hash_size,
                        uint8_t* signature, size_t* sig_size) {
    // Stubbed
    return false;
}

bool libspdm_ecdsa_verify(void* ec_context, size_t hash_nid,
                          const uint8_t* message_hash, size_t hash_size,
                          const uint8_t* signature, size_t sig_size) {
    // Stubbed
    return false;
}
