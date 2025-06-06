//
//  libspdm_rsa.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void* libspdm_rsa_new(void) {
    // Stubbed
    return NULL;
}

void libspdm_rsa_free(void* rsa_context) {
    // Stubbed
    return;
}

// Note to future implementors:
// `key_tag` is actually an enum value, libspdm_rsa_key_tag_t.
bool libspdm_rsa_set_key(void* rsa_context, const void* key_tag,
                         const uint8_t* big_number, size_t bn_size) {
    // Stubbed
    return false;
}
