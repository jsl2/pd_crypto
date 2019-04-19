//
//  Decryption.c
//  P&D
//
//  Created by Augustin Moutarlier on 28/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "decryption.h"
#include "galois_mult.h"
#include "array_copies.h"
#include "decryption_functions.h"
#include "encryption_functions.h"

void decryption(unsigned char *data, aes_key *key, unsigned char *buffer_out){
    static uint16_t T_e[8] = {0x0000};
    static unsigned char lenCd[16] = {0x00};
    static unsigned char IVd[12];
    static unsigned char Yd[16];
    static uint16_t Hd[8];
    static uint16_t T0d[8];
    static unsigned char Cid[16];
    static uint16_t Td[8];
   
    if (data[0] == 0x04) {
        packet_1_d(data, IVd);
        initialization(Yd, IVd, key, Hd, T0d, Td);
        seq_num_check(Yd, data);
    }
    else if (data[0] == 0x05) {
        packet_int_d(data, Cid);
        ghash_d(Hd, buffer_out, Cid, Yd, key, Td);
        seq_num_check(Yd, data);
        incr_lenC(lenCd);
    }
    else if (data[0] == 0x06) {
        packet_last_d(data, (unsigned char*) T_e);
        tag(Td, lenCd, T0d, Hd);
        tag_check((unsigned char *) Td, (unsigned char *) T_e);
    }
}
