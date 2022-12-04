//
// Created by marvolo on 04.12.22.
//

#include "aic_demo.h"
#include "../drv/dbgu.h"
#include "../lib/printf.h"

void print_timed_output(){
  while (1){
    char c = read_character();
    if(c == '$') {
      printf("Interrupt demo terminated.");
      break;
    }
    if(c != '\0') {
      for (int i = 0; i < 80; i++) {
        printf("%c", c);
        for (int j = 0; j < 100000; j++) {
          asm volatile("nop");
        }
      }
    }
  }
}