//
// Created by marvolo on 14.01.23.
//

#include "swi.h"
#include "../sys/thread.h"
#include "../demo/aic_demo.h"
#include "../lib/printf.h"


void swi_write_char(char character){
  register char output asm("r0");
  output = character;
  asm("swi #1");
}

char swi_read_char(){
  asm("swi #2");
  register char character asm ("r0");
  return character;
}

 void swi_create_thread(){
  asm("swi #4");
}

void swi_time_block(){
  asm("swi #5");
}


