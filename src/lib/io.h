//
// Created by Vico Tünke on 19.11.22.
//

#ifndef BETRIEBSSYSTEME_WS22_23_IO_H
#define BETRIEBSSYSTEME_WS22_23_IO_H


inline void write_u32(unsigned int addr, unsigned int val) {
    *(volatile unsigned int *) addr = val;
}

inline unsigned int read_u32(unsigned int addr) {
    return *(volatile unsigned int *) addr;
}

inline unsigned char read_u8(unsigned int addr) {
    return *(volatile unsigned char *) addr;
}

inline void write_u8(unsigned int addr, unsigned char val) {
    *(volatile unsigned int *) addr = val;
}

#endif //BETRIEBSSYSTEME_WS22_23_IO_H
