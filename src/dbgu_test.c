//
// Created by marvolo on 06.11.22.
//

#include "dbgu_test.h"
#include "dbgu.h"
#include "print.h"

void write_in_console(void){
  // Initialize DBGU
  enable_DBGU_receive();
  enable_DBGU_transmit();

//  char character = read_character();

  char string[] = "Hello World";

  print(string);


//  write_character(character);

  // write_buffer
  // read buffer
  // printf

}