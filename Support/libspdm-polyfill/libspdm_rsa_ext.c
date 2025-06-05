//
//  libspdm_rsa_ext.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stdbool.h>
#include <stdint.h>

bool libspdm_rsa_pss_sign(void* rsa_context, size_t hash_nid,
                          const uint8_t* message_hash, size_t hash_size,
                          uint8_t* signature, size_t* sig_size) {
    // Stubbed
    return false;
}

bool libspdm_rsa_pss_verify(void* rsa_context, size_t hash_nid,
                            const uint8_t* message_hash, size_t hash_size,
                            const uint8_t* signature, size_t sig_size) {
    // Stubbed
    return false;
}
