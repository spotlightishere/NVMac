//
//  libspdm_rsa.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stdbool.h>
#include <stdint.h>

bool libspdm_random_bytes(uint8_t* output, size_t size) {
    // Stubbed
    return true;
}

bool libspdm_random_seed(const uint8_t* seed, size_t seed_size) {
    // Stubbed
    // Ironically, this is what NVIDIA does as well
    return true;
}
