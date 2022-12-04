//
// Created by Vico Tünke on 30.11.22.
//

#include "aic.h"
#include "../lib/io.h"

#define AIC 0xFFFFF000

// Offsets
#define AIC_SMR1 0x40 // Source Mode Register Reserved for System Peripherals (ST, RTC, PMC, DBGU…)
#define AIC_IECR 0x120 // Interrupt Enable Command Register
#define AIC_SVR1 0x84 // Source Vector Register (Address of Handler)
#define AIC_EOICR 0x130 //  End of Interrupt Command Register
#define AIC_ICCR 0x128 // Interrupt Clear Command Register

#define FIQ 1 << 0
#define SYS 1 << 1

// Source Mode Register
//                              Internal Source         | External Source
#define AIC_SRCTYPE00 0 << 5 // High-level Sensitive    | Low-level Sensitive
#define AIC_SRCTYPE01 1 << 5 // Positive-edge Triggered | Negative-edge Triggered
#define AIC_SRCTYPE10 2 << 5 // High-level Sensitive    | High-level Sensitive
#define AIC_SRCTYPE11 3 << 5 // Positive-edge Triggered | Positive-edge Triggered


void init_sys_smr() {
  write_u32(AIC + AIC_SMR1, AIC_SRCTYPE00 | 7); // Source Type | Priority Level
}

void enable_sys_interrupt() {
  write_u32(AIC + AIC_IECR, SYS);
}

void set_sys_handler_address(unsigned int address){
  write_u32(AIC + AIC_SVR1, address);
}

void end_interrupt_request_handling(){
  write_u32(AIC + AIC_EOICR,1);
}

void clear_sys_interrupt(){
  write_u32(AIC + AIC_ICCR,SYS);
}
// Interrupt Vector register read -> Handling start
// End of Interrupt Command Register -> Handling end

