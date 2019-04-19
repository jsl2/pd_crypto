#include <stdint.h>
#include "lib/aes.h"
#include "packets.h"
#include "galois_mult.h"
#include "gcm.h"

void handle_packet(uint8_t *data, aes_key *key, uint8_t *buffer_out) {
    static uint16_t T_e[8] = {0x0000};
    static uint8_t lenCd[16] = {0x00};
    static uint8_t IVd[12];
    static uint8_t Yd[16];
    static uint16_t Hd[8];
    static uint16_t T0d[8];
    static uint8_t Cid[16];
    static uint16_t Td[8];

    if (data[0] == 0x04) {
        unpack_data_first(data, IVd);
        initialization(Yd, IVd, key, Hd, T0d, Td);
        seq_num_check(Yd, data);
    } else if (data[0] == 0x05) {
        unpack_data_int(data, Cid);
        gcm_decrypt(Hd, buffer_out, Cid, Yd, key, Td);
        seq_num_check(Yd, data);
        increment_lenC(lenCd);
    } else if (data[0] == 0x06) {
        unpack_data_last(data, (uint8_t *) T_e);
        tag(Td, lenCd, T0d, Hd);
        tag_check((uint8_t *) Td, (uint8_t *) T_e);
    }
}

/* packet_1 is an uint8_t of 17 elements */
void pack_data_first(uint8_t *Y, uint8_t *packet_1) {
    uint8_t i;
    packet_1[0] = 0x04;
    for (i = 1; i < 5; i++) {
        packet_1[i] = Y[11 + i];
    }
    for (i = 5; i < 17; i++) {
        packet_1[i] = Y[i - 5];
    }
}

/* packet_int is an uint8_t of 21 elements */
void pack_data_int(uint8_t *Y, uint8_t *Ci, uint8_t *packet_int) {
    uint8_t i;
    packet_int[0] = 0x05;
    for (i = 1; i < 5; i++) {
        packet_int[i] = Y[11 + i];
    }
    for (i = 5; i < 21; i++) {
        packet_int[i] = Ci[i - 5];
    }
}

/* packet_last is an uint8_t of 26 elements */
void pack_data_last(uint8_t *Y, uint8_t *T, uint8_t *packet_last) {
    uint8_t i;
    packet_last[0] = 0x06;
    for (i = 1; i < 5; i++) {
        packet_last[i] = Y[11 + i];
    }
    for (i = 5; i < 21; i++) {
        packet_last[i] = T[i - 5];
    }
}

/* Packet decoding to form the cipher texts */
void unpack_data_first(uint8_t *packet_1, uint8_t *IV) {
    uint8_t i;
    for (i = 0; i < 12; i++) {
        IV[i] = packet_1[i + 5];
    }
}

void unpack_data_int(uint8_t *packet_int, uint8_t *Ci) {
    uint8_t i;
    for (i = 0; i < 16; i++) {
        Ci[i] = packet_int[i + 5];
    }
}

void unpack_data_last(uint8_t *packet_last, uint8_t *Te) {
    uint8_t i;
    for (i = 0; i < 16; i++) {
        Te[i] = packet_last[i + 5];
    }
}

