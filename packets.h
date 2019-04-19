#ifndef CRYPTO_PACKETS_H
#define CRYPTO_PACKETS_H

void handle_packet(unsigned char *data, aes_key *key, unsigned char *buffer_out);

void pack_data_first(unsigned char *Y, unsigned char *packet_1);

void pack_data_int(unsigned char *Y, unsigned char *Ci, unsigned char *packet_int);

void pack_data_last(unsigned char *Y, unsigned char *Te, unsigned char *packet_last);

void unpack_data_first(unsigned char *packet_1, unsigned char *IV);

void unpack_data_int(unsigned char *packet_int, unsigned char *Ci);

void unpack_data_last(unsigned char *packet_last, unsigned char *Te);

#endif
