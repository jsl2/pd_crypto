//
//  main.c
//  P&D
//
//  Created by Valere MOUTARLIER on 7/03/19.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "aes.h"
#include "GHASH.h"

/*
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    printf("%08x\n", output[2]);
    return 0;
}
*/

/*
int main()
{
    aes_key key;
   // unsigned char output[16];
    uint16_t output[8];
    
    // This has to be done once at the begining of the code to set the key useable for the aes_encrypt key that is use to generate one signle block (the one called in GHASH file.
    aes_set_encrypt_key(&key, "\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c", 128);
 
    aes_encrypt(&key, "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96\xe9\x3d\x7e\x11\x73\x93\x17\x2a", output);
    
    printf("%08x\n", output[2]);
    if (memcmp(output, "\x3a\xd7\x7b\xb4\x0d\x7a\x36\x60\xa8\x9e\xca\xf3\x24\x66\xef\x97", 16) != 0) {
        fprintf(stderr, "Encryption failed\n");
        abort();
    }
    aes_set_decrypt_key(&key, "\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c", 128);
    
    aes_decrypt(&key, "\x3a\xd7\x7b\xb4\x0d\x7a\x36\x60\xa8\x9e\xca\xf3\x24\x66\xef\x97", output);
    
    if (memcmp(output, "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96\xe9\x3d\x7e\x11\x73\x93\x17\x2a", 16) != 0) {
        fprintf(stderr, "Decryption failed\n");
        abort();
    }
    return 0;
}
 */

int main()
{
    aes_key key;
    unsigned char Y[16];
    uint16_t ZA[8];
    uint16_t H[8];
    unsigned char Ti[16]= {0x00};
    uint16_t T1[8];
    unsigned char Ci[16]= {0x00};
    unsigned char Pi[16]= {0x00};
    uint16_t T[8] = {0x0000};
    unsigned char lenC[16];
    lenC[15]= 0x80;
    unsigned char IV[11] = {0x00};
    //uint16_t Z1[8] = {0x0000};
    //uint16_t H1[8] = {0x0001,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
    // unsigned char output[16];
    //uint16_t output[8];
    
    // This has to be done once at the begining of the code to set the key useable for the aes_encrypt key that is use to generate one signle block (the one called in GHASH file.
   
    aes_set_encrypt_key(&key, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 128);
    initialization(Y, IV, &key, H, T1);
    ghash(H, Pi, Ci, Y, Ti, &key, T);
    tag(T, lenC, T1, H);


    return 0;
}




