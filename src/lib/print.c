//
// Created by marvolo on 06.11.22.
//

#include "print.h"
#include "../drv/dbgu.h"
#include "swi.h"

void print(char string[]){
  char *traverse = string;
  while(*traverse != '\0')
  {
    write_character(*traverse);
//    swi_write_char(*traverse);
    traverse++;
  }
}