//
//  main.c
//  P&D
//
//  Created by Valere MOUTARLIER on 7/03/19.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//
// gcc -Wall -pedantic -ansi main.c aes.c PRBG.c Decryption.c Encryption.c Decryption_functions.c array_copies.c Galois_Multiplication.c Encryption_functions.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "aes.h"
#include "Encryption_functions.h"
#include "Decryption_functions.h"
#include "array_copies.h"
#include "Encryption.h"
#include "Decryption.h"
#include "Galois_Multiplication.h"

int main()
{
    aes_key key;
    uint16_t p2 = 0x0000;
    uint16_t p1 = 0x0000;
    uint16_t p2_out = 0x0000;
    uint16_t p1_out = 0x0000;
    uint8_t exception = 0x00;
    uint8_t i;
    uint8_t a;
    uint16_t h;
    unsigned short b;
    int j;
    unsigned char data_in_c[15];
    unsigned short data_in_s[8];
    unsigned char buffer_circ[128];
    unsigned char buffer_circ_d[128] = {0x0000};
    unsigned char data_out_t[16];
    unsigned char data_out_d[16];
    unsigned short data_to_speech[8];
    unsigned short buff_in[68];
    uint8_t data_ready = 0x00;
    uint8_t data_ready_out = 0x00;
   
    unsigned char IV[12];
    unsigned char Y[16] = {0x00};
    uint16_t H[8];
    uint16_t T0[8];
    unsigned char Ci[16]= {0x00};
    uint16_t T[8];
    unsigned char packet_1[17];
    unsigned char packet_int[21];
    unsigned char packet_last[21];
    unsigned char recovered_plaintexts[16];
    unsigned char lenC[16] = {0x00};

    long seed = time(NULL);
    srand((unsigned int)seed);

    for (h=0; h<68; h++) {
        b = rand();
        buff_in[h] = b;
    }
    for(j=0; j < 68; j++) {
        printf("%x ", buff_in[j]);
    }
    printf("\n");
    
    aes_set_encrypt_key(&key, (const unsigned char*)"\xfe\xf9\xe9\x92\x86\x65\x73\x1c\x7d\x6a\x8f\x94\x67\x30\x83\x08", 128);
    
    // INITIALIZATION
    for (h=0; h<12; h++) {
        a = rand();
        IV[h] = a;
    }
    initialization(Y, IV, &key, H, T0, T);
    data_1(Y, packet_1);
    decryption(packet_1, &key, recovered_plaintexts);
    
    // ENCRYPTION AND DECRYPTION CHAIN OF PLAINTEXTS
    
    printf("Recovered Plain text : \n");
    
    
    for (i = 0; i<9; i++) {
        for (h=0; h<8; h++) {
            data_in_s[h] = buff_in[h + 8*i];
        }
        shortToChar((unsigned char*)data_in_s, data_in_c);
        circularBuffer(data_in_c, buffer_circ, data_out_t, &p1, &p2, &data_ready, &exception);
    
        if (data_ready) {
            ghash_e(H, data_out_t, Ci, Y, &key, T);
            data_int(Y, Ci, packet_int);
            incr_lenC((unsigned short*)lenC);
            decryption(packet_int, &key, recovered_plaintexts);
        }
        if (data_ready) {
            circularBuffer_out(recovered_plaintexts, buffer_circ_d, data_out_d, &p1_out, &p2_out, &data_ready_out);
        }
        if (data_ready_out) {
            charToShort(data_out_d, (unsigned char*) data_to_speech);
            print_short(data_to_speech);
            // Send data_out_d
        }
    }
    
    // The following if concerns the case where the last packet of data from speech part + some extra leftovers from previously are less than 128 bits, it will be replaced by a condition on the value added on last packet from speech part.
    
    if ((p2>p1)|((p1>p2)&((128-p1+p2)>15))) {
        for (h=0; h<8; h++) {
            data_in_s[h] = 0x0000;
        }
        exception = 0x01;
        shortToChar((unsigned char*)data_in_s, data_in_c);
        circularBuffer(data_in_c, buffer_circ, data_out_t, &p1, &p2, &data_ready, &exception);
        ghash_e(H, data_out_t, Ci, Y, &key, T);
 //       incr(Y);        // TEST 1: SEQ NUM : incrementing Y for no packet send, will flag sequence order problem.
 //       Ci[10] = 0x00;  // TEST 2: Ci : changing cipher text after encryption and before decryption, will raise a flag concerning matching of tags (except if Ci[10] is 0x00 and then code should be re-run).
        data_int(Y, Ci, packet_int);
        incr_lenC((unsigned short*)lenC);
        decryption(packet_int, &key, recovered_plaintexts);
        circularBuffer_out(recovered_plaintexts, buffer_circ_d, data_out_d, &p1_out, &p2_out, &data_ready_out);
        charToShort(data_out_d, (unsigned char*) data_to_speech);
        print_short(data_to_speech);
        // Send data_out_d
    }
    inversion(lenC);
    tag(T, lenC, T0, H);
    data_last(Y,(unsigned char*) T, packet_last);
    printf("Buffer ready for new data\n");
    decryption(packet_last, &key, recovered_plaintexts);
    printf("\n");
    return 0;
}




