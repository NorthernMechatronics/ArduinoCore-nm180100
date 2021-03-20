#include <Arduino.h>

unsigned int counter = 0;
int incoming = 0;
void setup()
{
    pinMode(10, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    delay(1000);
    digitalToggle(10);
    Serial.println(counter++);

    incoming = Serial.peek();
    if (incoming > 0)
    {
        // say what you got:
        Serial.print("I received: ");
        Serial.println(incoming, DEC);

        Serial.read();
    }
}
