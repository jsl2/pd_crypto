//
//  Encryption.c
//  P&D
//
//  Created by Valere MOUTARLIER on 28/03/19.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "Encryption.h"
#include "aes.h"
#include "Galois_Multiplication.h"
#include "array_copies.h"
#include "Encryption_functions.h"

/*
void encryption(unsigned char *IV, aes_key *key, unsigned char *lenC, unsigned char *buffer_out, unsigned char *buffer_in, uint16_t *numBits){
    
    unsigned char Y[16] = {0x00};
    uint16_t H[8];
    uint16_t T0[8];
    unsigned char Ci[16]= {0x00};
    unsigned char Pi[16];
    uint16_t T[8];
    
    unsigned char packet_1[17];
    unsigned char packet_int[21];
    unsigned char packet_last[21];

    
    initialization(Y, IV, key, H, T0, T);
    data_1(Y, packet_1);
    int i;
    uint8_t np; // = NumOfElem / 16;  //np = number of plaintext Pi = np times 128 bits
    npIt(&numBits, &np);
    for (i=0; i<np; i++) {
        cbts(buffer_in, Pi, i);
        ghash_e(H, Pi, Ci, Y, key, T);
        data_int(Y, Ci, packet_int);
        cstb(Ci, buffer_out, i);  // This line should not be needed anymore after data are correctly sent one by one
    }
    tag(T, lenC, T0, H);
    data_last(Y,(unsigned char*) T, packet_last);
    printf("Buffer ready for new data\n");
}
 */
