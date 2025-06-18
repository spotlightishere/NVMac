//
//  libspdm_hkdf_sha.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool libspdm_hkdf_sha256_extract(const uint8_t* key, size_t key_size,
                                 const uint8_t* salt, size_t salt_size,
                                 uint8_t* prk_out, size_t prk_out_size) {
    return false;
}

bool libspdm_hkdf_sha256_expand(const uint8_t* prk, size_t prk_size,
                                const uint8_t* info, size_t info_size,
                                uint8_t* out, size_t out_size) {
    return false;
}

bool libspdm_hkdf_sha384_extract(const uint8_t* key, size_t key_size,
                                 const uint8_t* salt, size_t salt_size,
                                 uint8_t* prk_out, size_t prk_out_size) {
    return false;
}

bool libspdm_hkdf_sha384_expand(const uint8_t* prk, size_t prk_size,
                                const uint8_t* info, size_t info_size,
                                uint8_t* out, size_t out_size) {
    return false;
}

bool libspdm_hkdf_sha512_extract(const uint8_t* key, size_t key_size,
                                 const uint8_t* salt, size_t salt_size,
                                 uint8_t* prk_out, size_t prk_out_size) {
    return false;
}

bool libspdm_hkdf_sha512_expand(const uint8_t* prk, size_t prk_size,
                                const uint8_t* info, size_t info_size,
                                uint8_t* out, size_t out_size) {
    return false;
}
