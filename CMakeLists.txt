cmake_minimum_required(VERSION 3.10)
project(crypto C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-Wall -pedantic")

add_executable(test_crypto test.c mp_arithmetic.c mp_arithmetic.h lib/sha2.c lib/sha2.h bbs.c bbs.h lib/aes.c lib/aes.h
        galois_mult.c circular_buffer.c circular_buffer.h gcm.c gcm.h packets.c packets.h rsa_sig.c rsa_sig.h)