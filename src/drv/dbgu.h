//
// Created by marvolo on 05.11.22.
//

#ifndef BETRIEBSYSTEME_WS22_23_DBGU_H
#define BETRIEBSYSTEME_WS22_23_DBGU_H

void enable_DBGU_receive();
void disable_DBGU_receive();
void enable_DBGU_transmit();
void disable_DBGU_transmit();

int set_parity_mode(unsigned int);
int set_channel_mode();

char read_character();
void write_character(unsigned char);

#endif //BETRIEBSYSTEME_WS22_23_DBGU_H
