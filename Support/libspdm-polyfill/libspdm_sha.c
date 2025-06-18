//
//  libspdm_sha.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

void* libspdm_sha256_new(void) {
    // Stubbed
    return NULL;
}

void libspdm_sha256_free(void* sha256_ctx) {
    // Stubbed
    return;
}

bool libspdm_sha256_init(void* sha256_context) {
    // Stubbed
    return false;
}

bool libspdm_sha256_duplicate(const void* sha256_context,
                              void* new_sha256_context) {
    // Stubbed
    return false;
}

bool libspdm_hmac_sha256_update(void* hmac_sha256_ctx, const void* data,
                                size_t data_size) {
    // Stubbed
    return false;
}

bool libspdm_sha256_update(void* sha256_context, const void* data,
                           size_t data_size) {
    // Stubbed
    return false;
}

bool libspdm_sha256_final(void* sha256_context, uint8_t* hash_value) {
    // Stubbed
    return false;
}

bool libspdm_sha256_hash_all(const void* data, size_t data_size,
                             uint8_t* hash_value) {
    // Stubbed
    return false;
}

void* libspdm_sha384_new(void) {
    // Stubbed
    return NULL;
}

void libspdm_sha384_free(void* sha384_ctx) {
    // Stubbed
    return;
}

bool libspdm_sha384_init(void* sha384_context) {
    // Stubbed
    return false;
}

bool libspdm_sha384_duplicate(const void* sha384_context,
                              void* new_sha384_context) {
    // Stubbed
    return false;
}

bool libspdm_sha384_update(void* sha384_context, const void* data,
                           size_t data_size) {
    // Stubbed
    return false;
}

bool libspdm_sha384_final(void* sha384_context, uint8_t* hash_value) {
    // Stubbed
    return false;
}

bool libspdm_sha384_hash_all(const void* data, size_t data_size,
                             uint8_t* hash_value) {
    // Stubbed
    return false;
}
