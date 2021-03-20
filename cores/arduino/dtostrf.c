#include <stdio.h>

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  __asm__(".global _printf_float");

  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
