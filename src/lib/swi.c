//
// Created by marvolo on 14.01.23.
//

#include "swi.h"
#include "../sys/thread.h"
#include "../demo/aic_demo.h"
#include "../lib/printf.h"


void swi_write_char(char character){
  asm("swi #1");
}

char swi_read_char(){
  asm("swi #2");
  register char character asm ("r0");
  return character;
}



 void swi_create_thread(char character){
   asm volatile("mov r0, %[character] \n\t"
                "swi #4 \n\t"
       :
       :[character] "r" (character)
   :);
}

void swi_time_block(){
  asm("swi #5");
}


