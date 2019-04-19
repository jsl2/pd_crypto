#ifndef CRYPTO_GCM_H
#define CRYPTO_GCM_H

void initialization(uint8_t *Y, uint8_t *IV, aes_key *key, uint16_t *H, uint16_t *T0, uint16_t *T);

void gcm_encrypt(uint16_t *H, uint8_t *Pi, uint8_t *Ci, uint8_t *Y, aes_key *key, uint16_t *T);

void tag(uint16_t *T, uint8_t *lenC, uint16_t *T0, uint16_t *H);

void seq_num_check(uint8_t *Y, uint8_t *packet);

void gcm_decrypt(uint16_t *H, uint8_t *Pi, uint8_t *Ci, uint8_t *Y, aes_key *key, uint16_t *T);

uint8_t tag_check(uint8_t *T, uint8_t *Te);

#endif
