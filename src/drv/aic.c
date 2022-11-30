//
// Created by Vico Tünke on 30.11.22.
//

#include "aic.h"
#include "../lib/io.h"

#define AIC 0xFFFFF000

// Offsets
#define AIC_SMR1 0x40
#define AIC_IECR 0x120

#define FIQ 1 << 0
#define SYS 1 << 1

// Source Mode Register
#define AIC_SRCTYPE00 0 << 5
#define AIC_SRCTYPE01 1 << 5
#define AIC_SRCTYPE10 2 << 5
#define AIC_SRCTYPE11 3 << 5


void init_smr1() {
  write_u32(AIC + AIC_SMR1, AIC_SRCTYPE00 | 7); // Source Type | Priority Level
}

void init_sys_interrupt() {
  write_u32(AIC + AIC_IECR, SYS);
}

