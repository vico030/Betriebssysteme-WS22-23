//
// Created by marvolo on 05.11.22.
//

#ifndef BETRIEBSYSTEME_WS22_23_DBGU_H
#define BETRIEBSYSTEME_WS22_23_DBGU_H

void enable_DBGU_receive();
void disable_DBGU_receive();
void enable_DBGU_transmit();
void disable_DBGU_transmit();
void enable_DBGU_interrupt();
void disable_DBGU_interrupt();

int set_parity_mode(unsigned int);
int set_channel_mode();

char read_character();
void write_character(unsigned char);

void push_to_lq(void);
char pop_from_lq(void);
char peek_at_lq(void);

void init_DBGU();

int  is_readable(void);
#endif //BETRIEBSYSTEME_WS22_23_DBGU_H
