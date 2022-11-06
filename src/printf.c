#include "stdarg.h"
#include "dbgu.h"
#include "print.h"


static char *convert(unsigned int num, int base){
  static char hex_representation[]= "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  ptr = &buffer[49];
  *ptr = '\0';

  do
  {
    *--ptr = hex_representation[num % base];
    num /= base;
  } while(num != 0);

  return(ptr);
}

__attribute__((format(printf, 1, 2)))
void printf(char *fmt, ...) {
  char *traverse;
  unsigned int i;
  char *s;

  va_list args;
  va_start(args, fmt);

  traverse = fmt;

  while(*traverse != '\0')
  {
    char next_char = *traverse;
    traverse++;

    if(next_char != '%')
    {
      write_character(next_char);
      continue;
    }
    next_char = *traverse;
    traverse++;

    switch(next_char)
    {
      case 'c' :
        i = va_arg(args, unsigned int );           // Fetch char
        write_character(i);
        break;

      case 's':
        s = va_arg(args, char *);        // Fetch string
        print(s);
        break;

      case 'x':
        i = va_arg(args, unsigned int);  // Fetch hexadecimal representation
        print(convert(i, 16));
        break;

      case 'p':
        i = (unsigned int) va_arg(args, void*);         // Fetch hexadecimal representation
        print(convert(i, 16));
        break;
    }
  }
  va_end(args);
}