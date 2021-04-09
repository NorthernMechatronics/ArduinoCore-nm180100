#include <Arduino.h>
#include <Servo.h>

Servo s1;

void setup()
{
    s1.attach(6);
    s1.write(170);
}

void loop()
{
}

