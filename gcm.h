#ifndef CRYPTO_GCM_H
#define CRYPTO_GCM_H

void initialization(unsigned char *Y, unsigned char *IV, aes_key *key, uint16_t *H, uint16_t *T0, uint16_t *T);

void gcm_encrypt(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T);

void tag(uint16_t *T, unsigned char *lenC, uint16_t *T0, uint16_t *H);

void seq_num_check(unsigned char *Y, unsigned char *packet);

void gcm_decrypt(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T);

uint8_t tag_check(unsigned char *T, unsigned char *Te);

#endif
