#ifndef CRYPTO_PACKETS_H
#define CRYPTO_PACKETS_H

void handle_packet(uint8_t *data, aes_key *key, uint8_t *buffer_out);

void pack_data_first(uint8_t *Y, uint8_t *packet_1);

void pack_data_int(uint8_t *Y, uint8_t *Ci, uint8_t *packet_int);

void pack_data_last(uint8_t *Y, uint8_t *Te, uint8_t *packet_last);

void unpack_data_first(uint8_t *packet_1, uint8_t *IV);

void unpack_data_int(uint8_t *packet_int, uint8_t *Ci);

void unpack_data_last(uint8_t *packet_last, uint8_t *Te);

#endif
