//
//  libspdm_x509.c
//  spdm-polyfill
//
//  Created by Spotlight Deveaux on 2025-06-04.
//

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

bool libspdm_x509_construct_certificate(const uint8_t* cert, size_t cert_size,
                                        uint8_t** single_x509_cert) {
    // Stubbed
    return false;
}

bool libspdm_x509_construct_certificate_stack(uint8_t** x509_stack, ...) {
    // Stubbed
    return false;
}

void libspdm_x509_free(void* x509_cert) {
    // Stubbed
    return;
}

void libspdm_x509_stack_free(void* x509_stack) {
    // Stubbed
    return;
}

bool libspdm_asn1_get_tag(uint8_t** ptr, const uint8_t* end, size_t* length,
                          uint32_t tag) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_subject_name(const uint8_t* cert, size_t cert_size,
                                   uint8_t* cert_subject,
                                   size_t* subject_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_common_name(const uint8_t* cert, size_t cert_size,
                                  char* common_name, size_t* common_name_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_organization_name(const uint8_t* cert, size_t cert_size,
                                        char* name_buffer,
                                        size_t* name_buffer_size) {
    // Stubbed
    return false;
}

bool libspdm_rsa_get_public_key_from_x509(const uint8_t* cert, size_t cert_size,
                                          void** rsa_context) {
    // Stubbed
    return false;
}

bool libspdm_ec_get_public_key_from_x509(const uint8_t* cert, size_t cert_size,
                                         void** ec_context) {
    // Stubbed
    return false;
}

bool libspdm_ecd_get_public_key_from_x509(const uint8_t* cert, size_t cert_size,
                                          void** ecd_context) {
    // Stubbed
    return false;
}

bool libspdm_sm2_get_public_key_from_x509(const uint8_t* cert, size_t cert_size,
                                          void** sm2_context) {
    // Stubbed
    return false;
}

bool libspdm_x509_verify_cert(const uint8_t* cert, size_t cert_size,
                              const uint8_t* ca_cert, size_t ca_cert_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_verify_cert_chain(const uint8_t* root_cert,
                                    size_t root_cert_length,
                                    const uint8_t* cert_chain,
                                    size_t cert_chain_length) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_cert_from_cert_chain(const uint8_t* cert_chain,
                                           size_t cert_chain_length,
                                           const int32_t cert_index,
                                           const uint8_t** cert,
                                           size_t* cert_length) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_tbs_cert(const uint8_t* cert, size_t cert_size,
                               uint8_t** tbs_cert, size_t* tbs_cert_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_version(const uint8_t* cert, size_t cert_size,
                              size_t* version) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_serial_number(const uint8_t* cert, size_t cert_size,
                                    uint8_t* serial_number,
                                    size_t* serial_number_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_issuer_name(const uint8_t* cert, size_t cert_size,
                                  uint8_t* cert_issuer, size_t* issuer_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_issuer_common_name(const uint8_t* cert, size_t cert_size,
                                         char* common_name,
                                         size_t* common_name_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_issuer_orgnization_name(const uint8_t* cert,
                                              size_t cert_size,
                                              char* name_buffer,
                                              size_t* name_buffer_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_signature_algorithm(const uint8_t* cert, size_t cert_size,
                                          uint8_t* oid, size_t* oid_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_extension_data(const uint8_t* cert, size_t cert_size,
                                     const uint8_t* oid, size_t oid_size,
                                     uint8_t* extension_data,
                                     size_t* extension_data_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_validity(const uint8_t* cert, size_t cert_size,
                               uint8_t* from, size_t* from_size, uint8_t* to,
                               size_t* to_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_key_usage(const uint8_t* cert, size_t cert_size,
                                size_t* usage) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_extended_key_usage(const uint8_t* cert, size_t cert_size,
                                         uint8_t* usage, size_t* usage_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_get_extended_basic_constraints(
    const uint8_t* cert, size_t cert_size, uint8_t* basic_constraints,
    size_t* basic_constraints_size) {
    // Stubbed
    return false;
}

bool libspdm_x509_set_date_time(char const* date_time_str, void* date_time,
                                size_t* date_time_size) {
    // Stubbed
    return false;
}

int32_t libspdm_x509_compare_date_time(const void* date_time1,
                                       const void* date_time2) {
    // Stubbed
    // However, function was already stubbed by Nvidia
    return -3;
}

bool libspdm_gen_x509_csr(size_t hash_nid, size_t asym_nid,
                          uint8_t* requester_info, size_t requester_info_length,
                          void* context, char* subject_name, size_t* csr_len,
                          uint8_t** csr_pointer) {
    // Stubbed
    return false;
}
