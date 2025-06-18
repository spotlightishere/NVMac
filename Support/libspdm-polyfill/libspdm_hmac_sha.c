//
//  libspdm_hmac_sha.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include "libspdm_polyfill.h"

void* libspdm_hmac_sha256_new(void) {
    // Stubbed
    return NULL;
}

void libspdm_hmac_sha256_free(void* hmac_sha256_ctx) {
    // Stubbed
    return;
}

bool libspdm_hmac_sha256_set_key(void* hmac_sha256_ctx, const uint8_t* key,
                                 size_t key_size) {
    // Stubbed
    return false;
}

bool libspdm_hmac_sha256_duplicate(const void* hmac_sha256_ctx,
                                   void* new_hmac_sha256_ctx) {
    // Stubbed
    return false;
}
bool libspdm_hmac_sha256_final(void* hmac_sha256_ctx, uint8_t* hmac_value) {
    // Stubbed
    return false;
}

bool libspdm_hmac_sha256_all(const void* data, size_t data_size,
                             const uint8_t* key, size_t key_size,
                             uint8_t* hmac_value) {
    // Stubbed
    return false;
}

void* libspdm_hmac_sha384_new(void) {
    // Stubbed
    return NULL;
}

void libspdm_hmac_sha384_free(void* hmac_sha384_ctx) {
    // Stubbed
    return;
}

bool libspdm_hmac_sha384_set_key(void* hmac_sha384_ctx, const uint8_t* key,
                                 size_t key_size) {
    // Stubbed
    return false;
}

bool libspdm_hmac_sha384_duplicate(const void* hmac_sha384_ctx,
                                   void* new_hmac_sha384_ctx) {
    // Stubbed
    return false;
}

bool libspdm_hmac_sha384_update(void* hmac_sha384_ctx, const void* data,
                                size_t data_size) {
    // Stubbed
    return false;
}

bool libspdm_hmac_sha384_final(void* hmac_sha384_ctx, uint8_t* hmac_value) {
    // Stubbed
    return false;
}

bool libspdm_hmac_sha384_all(const void* data, size_t data_size,
                             const uint8_t* key, size_t key_size,
                             uint8_t* hmac_value) {
    // Stubbed
    return false;
}
