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

  char character = 'C';
  char string[] = "Linnert";
  int hex = 233; // == E9
  void * vptr = &hex;

  char character_format[] = "Hello World! Character: %c was printed.\r\n";
  char string_format[] = "Hello World! String: %s was printed.\r\n";
  char hex_format[] = "Hello World! Hex: %x was printed.\r\n";
  char void_format[] = "Hello World! Void: %p was printed.\r\n";


  printf(character_format, character);
  printf(string_format, string);
  printf(hex_format, hex);
  printf(void_format, vptr);

//  printf(character_format);
//  printf(character_format);
//  printf(character_format);
//  printf(character_format);



//  write_character(character);

  // write_buffer
  // read buffer
  // printf

}