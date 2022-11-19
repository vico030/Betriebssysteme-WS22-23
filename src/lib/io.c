//
// Created by Vico Tünke on 19.11.22.
//

#include "io.h"

inline unsigned int read_u32(unsigned int addr) {
    return *(volatile unsigned int *) addr;
}

inline void write_u32(unsigned int addr, unsigned int val) {
    *(volatile unsigned int *) addr = val;
}

inline unsigned char read_u8(unsigned int addr) {
    return *(volatile unsigned char *) addr;
}

inline void write_u8(unsigned int addr, unsigned char val) {
    *(volatile unsigned int *) addr = val;
}