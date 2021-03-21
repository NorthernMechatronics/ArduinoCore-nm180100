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

    incoming = Serial.read();
    if (incoming == 's')
    {
        int voltage_code = analogRead(GPIO29);
        // say what you got:
        Serial.print("Analog Code: ");
        Serial.println(voltage_code, DEC);
    }
}
