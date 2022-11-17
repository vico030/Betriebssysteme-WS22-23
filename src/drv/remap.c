// check how to trigger interrupt -> access undefined memory
// trigger abort interrupt by accessing undefined memory address
 unsigned int trigger_abort_interrupt() {
   return  *(volatile unsigned int*)0x90000000;
}


// where is r9-14 (Doc 8. Memories)


//remap interrupt vector table by setting the RCB (Remap Command Bit) to 1 in the 16.4.1 MC Remap Control Register
//at base address 0xFFFF FF00

//asm vector table muss in z.B. asm definiert werden und dann via einer eigenen section im start.c verschoben werden
// davor muss vorher im RCB das bit auf 1 gesetzt werden


// nach dem vector table verschoben wurde müssen routinen erstellt werden auf die von der IVT gejumped werden um diese
// auszuführen





