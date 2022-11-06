//
// Created by marvolo on 06.11.22.
//

#include "print.h"
#include "../drv/dbgu.h"

void print(char string[]){
  char *traverse = string;
  while(*traverse != '\0')
  {
    write_character(*traverse);
    traverse++;
  }
}