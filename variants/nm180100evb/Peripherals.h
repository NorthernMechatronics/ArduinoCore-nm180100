#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "HardwareSerial.h"

extern HardwareSerial Serial;
extern void serialEvent() __attribute((weak));

extern HardwareSerial Serial1;
extern void serialEvent1() __attribute((weak));


#endif /* PERIPHERALS_H */
