//
// Created by Vico Tünke on 30.11.22.
//

#include "st.h"
#include "../lib/io.h"

#define ST 0xFFFFFD00 // System Timer base Address

// Offsets
#define ST_SR 0x0010 // offset System Timer Status Register
#define ST_IER 0x0014 // offset System Timer Interrupt Enable Register
#define ST_PIMR 0x0004 // Period Interval Mode Register


#define ST_PITS 1 << 0


void enable_period_interval_interrupt() {
  write_u32(ST + ST_IER, ST_PITS);
}

int set_period_interval(unsigned short interval) {
  write_u16(ST + ST_PIMR, interval);
}