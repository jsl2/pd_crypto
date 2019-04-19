test_crypto : aes.o sha2.o
	cc -o test_crypto test.c bbs.c circular_buffer.c galois_mult.c gcm.c \
	mp_arithmetic.c packets.c rsa_sig.c aes.o sha2.o -Wall -pedantic -ansi

aes.o : lib/aes.c lib/aes.h
	cc -c lib/aes.c
sha2.o : lib/sha2.h lib/sha2.h
	cc -c lib/sha2.c

clean :
	rm test_crypto aes.o sha2.o
