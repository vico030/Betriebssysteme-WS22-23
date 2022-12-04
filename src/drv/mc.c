#include "../lib/io.h"

#define MC 0xFFFFFF00 // Memory Controller

// Offsets
#define MC_RCR 0x00 // MC Remap Control Register
#define MC_ASR 0x04 // MC Abort Status Register
#define MC_AASR 0x08 // MC Abort Address Status Register
#define MC_MPR 0x0C // MC Master Priority Register

// MC Remap Control Register
#define RCB 1 << 0 // Remap Command Bit Toggle

void enable_MC_remap(void) {
    write_u32(MC + MC_RCR, RCB);
}

unsigned int get_abort_address(void) {
  return read_u32(MC + MC_AASR);
}



