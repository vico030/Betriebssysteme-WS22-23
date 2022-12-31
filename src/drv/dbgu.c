#include "dbgu.h"
#include "../lib/io.h"
#include "../lib/loop_queue.h"

#define DBGU 0xFFFFF200

// Offsets
#define DBGU_CR 0x0000 // Control Register Write-only -
#define DBGU_MR 0x0004 // Mode Register Read/Write 0x0
#define DBGU_IER 0x0008 // Interrupt Enable Register Write-only –
#define DBGU_IDR 0x000C // Interrupt Disable Register Write-only –
#define DBGU_IMR 0x0010 // Interrupt Mask Register Read-only 0x0
#define DBGU_SR 0x0014 // Status Register Read-only –
#define DBGU_RHR 0x0018 // Receive Holding Register Read-only 0x0
#define DBGU_THR 0x001C // Transmit Holding Register Write-only –
#define DBGU_BRGR 0x0020 // Baud Rate Generator Register Read/Write 0x0

// Control Register
#define RSTRX 1 << 2 // Reset Receiver
#define RSTTX 1 << 3 // Reset Transmitter
#define RXEN 1 << 4 // Receiver Enable
#define RXDIS 1 << 5 // Receiver Disable
#define TXEN 1 << 6 // Transmitter Enable
#define TXDIS 1 << 7 // Transmitter Disable
#define RSTSTA 1 << 8 // Reset Status Bits

// Mode Register
#define PAR_EVEN (0 << 11) | (0 << 10) | (0 << 9)
#define PAR_ODD (0 << 11) | (0 << 10) | (1 << 9)
#define PAR_SPACE (0 << 11) | (1 << 10) | (0 << 9)
#define PAR_MARK (0 << 11) | (1 << 10) | (1 << 9)
#define PAR_NONE (1 << 11)
#define CHMODE_NORMAL (0 << 15) | (0 << 14)
#define CHMODE_ECHO (0 << 15) | (1 << 14)
#define CHMODE_LOOPL (1 << 15) | (0 << 14)
#define CHMODE_LOOPR (1 << 15) | (1 << 14)

// Status Register
#define RXRDY 1 << 0 // Receiver Ready
#define TXRDY 1 << 1 // Transmitter Ready
#define ENDRX 1 << 3 // End of Receiver Transfer
#define ENDTX 1 << 4 // End of Transmitter Transfer
#define OVRE 1 << 5 // Overrun Error
#define FRAME 1 << 6 // Framing Error
#define PARE 1 << 7 // Parity Error
#define TXEMPTY 1 << 9 // Transmitter Empty
#define TXBUFE 1 << 11 // Transmission Buffer Empty
#define RXBUFF 1 << 12 // Receive Buffer Full
#define COMMTX 1 << 30 // Debug Communication Channel Write Status
#define COMMRX 1 << 31 // Debug Communication Channel Read Status


char test_buffer[256];
loop_queue DBGU_LQ = {buffer_size, 0,0,0,test_buffer};
//static loop_queue DBGU_LQ;

inline void enable_DBGU_receive(void) {
    write_u32(DBGU + DBGU_CR, RXEN);
}

inline void disable_DBGU_receive(void) {
    write_u32(DBGU + DBGU_CR, RXDIS);
}

inline void enable_DBGU_transmit(void) {
    write_u32(DBGU + DBGU_CR, TXEN);
}

inline void disable_DBGU_transmit(void) {
    write_u32(DBGU + DBGU_CR, TXDIS);
}

inline void enable_DBGU_interrupt(void) {
  write_u32(DBGU + DBGU_IER, RXRDY);
}

inline void disable_DBGU_interrupt(void){
  write_u32(DBGU + DBGU_IDR, RXRDY);
}

inline int set_parity_mode(unsigned int mode) {
    if (
            mode == (PAR_EVEN) ||
            mode == (PAR_ODD) ||
            mode == (PAR_SPACE) ||
            mode == (PAR_MARK) ||
            mode == (PAR_NONE)
            ) {
        write_u32(DBGU + DBGU_MR, mode);
        return 0;
    }
    return 1;
}

inline int set_channel_mode(unsigned int mode) {
    if (
            mode == (CHMODE_NORMAL) ||
            mode == (CHMODE_ECHO) ||
            mode == (CHMODE_LOOPL) ||
            mode == (CHMODE_LOOPR)
            ) {
        write_u32(DBGU + DBGU_MR, mode);
        return 0;
    }
    return 1;
}


inline void init_DBGU(){
//  char test_buffer[256];
//  loop_queue DBGU_LQ = {buffer_size, 0,0,0,test_buffer};

  enable_DBGU_interrupt();
  enable_DBGU_transmit();
  enable_DBGU_receive();
}

inline char read_character(void) {
    //while ((read_u32(DBGU + DBGU_SR) & RXRDY) == 0);
    //return (char) read_u32(DBGU + DBGU_RHR);
    if(DBGU_LQ.size_content > 0) return lq_pop(&DBGU_LQ);
    return '\0';
}

inline void push_to_lq(){
  lq_push(&DBGU_LQ, (char) read_u32(DBGU + DBGU_RHR));
}

inline char pop_from_lq(){
  return lq_pop(&DBGU_LQ);
}

inline char peek_at_lq(){
  return lq_peek(&DBGU_LQ);
}

inline void write_character(unsigned char character) {
    while ((read_u32(DBGU + DBGU_SR) & TXRDY) == 0);
    write_u8(DBGU + DBGU_THR, character);
}

int is_readable(){
  return read_u32(DBGU + DBGU_SR) & RXRDY;
}

