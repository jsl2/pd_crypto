// RFC8017 PCKS #1 2.2

#ifndef CRYPTO_RSA_SIG_H
#define CRYPTO_RSA_SIG_H

void
pkcs_sign(uint16_t *n, uint16_t *n_prime, uint16_t *d, uint16_t *R, uint16_t *part1, uint16_t *part2, uint16_t *sig);

uint32_t
pkcs_verify(uint16_t *n, uint16_t *n_prime, uint16_t *e, uint16_t *R, uint16_t *part1, uint16_t *part2, uint16_t *sig);

#endif //CRYPTO_RSA_SIG_H
