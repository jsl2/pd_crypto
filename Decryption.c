//
//  Decryption.c
//  P&D
//
//  Created by Augustin Moutarlier on 28/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "Decryption.h"
#include "Galois_Multiplication.h"
#include "array_copies.h"
#include "Decryption_functions.h"
#include "Encryption_functions.h"
#include "Shared.h"

void decryption(unsigned char *data, aes_key *key, unsigned char *buffer_out){
    
    uint16_t T_e[8] = {0x0000};
   
    if (data[0] == 0x04) {
        packet_1_d(data, IVd);
        initialization(Yd, IVd, key, Hd, T0d, Td);
        seqNumCheck(Yd, data);
    }
    else if (data[0] == 0x05) {
        packet_int_d(data, Cid);
        ghash_d(Hd, Pid, Cid, Yd, key, Td);
        seqNumCheck(Yd, data);
        incr_lenC((unsigned short*)lenCd);
        charToShort(Pid, (unsigned char*) buff_out);
        shortToChar((unsigned char*)buff_out, buffer_out);
        jd++;
    }
    else if (data[0] == 0x06) {
        packet_last_d(data, (unsigned char*) T_e);
        inversion(lenCd);
        tag(Td, lenCd, T0d, Hd);
        tagCheck((unsigned char*)Td, (unsigned char*)T_e);
        charToShort(buffer_out, (unsigned char*) buff_out);
        printf("Data ready to be used\n");
    }
}
