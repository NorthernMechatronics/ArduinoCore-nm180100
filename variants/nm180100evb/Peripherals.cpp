#include "Peripherals.h"

HardwareSerial Serial(GPIO22, GPIO23);
void serialEvent() __attribute__((weak));

HardwareSerial Serial1(GPIO24, GPIO25);
void serialEvent1() __attribute__((weak));

void serialEventRun()
{
    if (Serial.available())
    {
        serialEvent();
    }
    if (Serial1.available())
    {
        serialEvent1();
    }
}
