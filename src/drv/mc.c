#include "../lib/io.h"

#define MC 0xFFFFFF00 // Memory Controller

// Offsets
#define MC_RCR 0x00 // MC Remap Control Register
#define MC_ASR 0x04 // MC Abort Status Register
#define MC_AASR 0x08 // MC Abort Address Status Register
#define MC_MPR 0x0C // MC Master Priority Register

// MC Remap Control Register
#define RCB 1 << 0 // Remap Command Bit Toggle

// where is r9-14 (Doc 8. Memories)


//remap interrupt vector table by setting the RCB (Remap Command Bit) to 1 in the 16.4.1 MC Remap Control Register
//at base address 0xFFFF FF00

void enable_MC_remap(void) {
    write_u32(MC + MC_RCR, RCB);
}

//asm vector table muss in z.B. asm definiert werden und dann via einer eigenen section im start.c verschoben werden
// davor muss vorher im RCB das bit auf 1 gesetzt werden


// nach dem vector table verschoben wurde müssen routinen erstellt werden auf die von der IVT gejumped werden um diese
// auszuführen





