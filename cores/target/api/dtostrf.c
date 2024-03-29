/*
  dtostrf - Emulation for dtostrf function from avr-libc
  Copyright (c) 2016 Arduino LLC.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// This is a default implementation for dtostrf function.
// This file should be used if the standard lib doesn't provide an
// implementation of dtostrf.

// Create a file called "dtostrf.c" with the following include:
// #include "api/deprecated-avr-comp/avr/dtostrf.c.impl"

#include <am_util.h>

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  // default sprintf calls malloc and it is not re-entrant
  // sprintf(fmt, "%%%d.%df", width, prec);
  // sprintf(sout, fmt, val);
  am_util_stdio_sprintf(fmt, "%%%d.%df", width, prec);
  am_util_stdio_sprintf(sout, fmt, val);
  return sout;
}

