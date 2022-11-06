#include "<stdarg.h>"
#include "<stdio.h>"

__attribute__((format(printf, 1, 2)))
void printf(char *fmt, ...) {
  char *traverse;
  unsigned int i;
  char *s;

  va_list args;
  va_start(args, fmt);

  for(traverse = fmt; *traverse != '\0'; traverse++)
  {
    while(*traverse != '%')
    {
      putchar(*traverse);
      traverse++;
    }

    traverse++;

    switch(*traverse)
    {
      case 'c' :
        i = va_arg(args, int);           // Fetch char
        putchar(i);
        break;

      case 's':
        s = va_arg(args, char *);        // Fetch string
        puts(s);
        break;

      case 'x':
        i = va_arg(args, unsigned int);  // Fetch hexadecimal representation
        puts(convert(i, 16));
        break;

      case 'p':
        i = va_arg(args, void*);         // Fetch hexadecimal representation
        puts(convert(i, 16));
        break;
    }
  }
  va_end(args);
}

char *convert(unsigned int num, int base)
{
  static char representation[]= "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  ptr = &buffer[49];
  *ptr = '\0';

  do
  {
    *--ptr = representation[num % base];
    num /= base;
  } while(num != 0);

  return(ptr);
}