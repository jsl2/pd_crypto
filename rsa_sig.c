// RFC8017 PCKS #1 2.2
#include <stdint.h>
#include "lib/sha2.h"
#include "rsa_sig.h"
#include "mp_arithmetic.h"

#define T_PREFIX_LEN 19
#define T_LEN (T_PREFIX_LEN + SHA256_DIGEST_LENGTH)

/* Prefix for DER encoding for SHA-256 (EMSA-PKCS1-v1_5) */
uint8_t T_prefix[T_PREFIX_LEN] = {0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02,
                                  0x01,
                                  0x05, 0x00, 0x04, 0x20};

void pkcs_encode(uint8_t *M, uint16_t em_len, uint8_t *EM) {
    uint16_t i;
    EM[1] = 1;
    for (i = 0; i < em_len - T_LEN - 3; i++) // remove magic number
        EM[2 + i] = 0xFF;

    for (i = 0; i < T_PREFIX_LEN; i++) // remove magic number (tlen)
        EM[em_len - T_LEN + i] = T_prefix[i];

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        EM[em_len - SHA256_DIGEST_LENGTH + i] = M[i];
}

void os2ip(uint8_t *EM, uint16_t *m) {
    uint16_t i;
    m[0] = 64;
    for (i = 0; i < 64; i++) {
        m[i + 1] = EM[127 - 2 * i];
        m[i + 1] |= (EM[126 - 2 * i] << 8u);
    }
}

/* RSASSA-PKCS1-V1_5-SIGN. Message M split into two parts as STS always concatenates two exponentials for signature */
void
pkcs_sign(uint16_t *n, uint16_t *n_prime, uint16_t *d, uint16_t *R, uint16_t *part1, uint16_t *part2, uint16_t *sig) {
    SHA256_CTX ctx;
    uint8_t hash[SHA256_DIGEST_LENGTH] = {0};
    uint8_t EM[128] = {0};
    uint16_t m[MAX_SIZE] = {0};

    SHA256_Init(&ctx);
    SHA256_Update(&ctx, (const uint8_t *) (&part1[1]), (size_t) part1[0] * 2);
    SHA256_Update(&ctx, (const uint8_t *) (&part2[1]), (size_t) part2[0] * 2);
    SHA256_Final(hash, &ctx);
    pkcs_encode(hash, 128, EM);
    os2ip(EM, m);

    dh_mon_exp(m, d, n, n_prime, R, sig);
}

uint32_t
pkcs_verify(uint16_t *n, uint16_t *n_prime, uint16_t *e, uint16_t *R, uint16_t *part1, uint16_t *part2, uint16_t *sig) {
    SHA256_CTX ctx;
    uint8_t hash[SHA256_DIGEST_LENGTH] = {0};
    uint8_t EM[128] = {0};
    uint16_t m[MAX_SIZE] = {0};
    uint16_t verif[MAX_SIZE] = {0};

    SHA256_Init(&ctx);
    SHA256_Update(&ctx, (const uint8_t *) (&part1[1]), (size_t) part1[0] * 2);
    SHA256_Update(&ctx, (const uint8_t *) (&part2[1]), (size_t) part2[0] * 2);
    SHA256_Final(hash, &ctx);
    pkcs_encode(hash, 128, EM);
    os2ip(EM, m);

    dh_mon_exp(sig, e, n, n_prime, R, verif);
    return is_equal(m, verif);
}