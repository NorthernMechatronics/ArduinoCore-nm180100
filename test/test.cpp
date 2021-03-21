#include <Arduino.h>

unsigned int counter = 0;
int brightness = 10;
int incoming = 0;
void setup()
{
    pinMode(LED4, OUTPUT);
    analogWrite(GPIO30, brightness);
    Serial.begin(115200);
}

void loop()
{
    delay(1000);
    digitalToggle(LED4);
    Serial.println(counter++);

    incoming = Serial.read();
    switch(incoming)
    {
    case 's':
    {
        int voltage_code = analogRead(GPIO29);
        // say what you got:
        Serial.print("Analog Code: ");
        Serial.println(voltage_code, DEC);
    }
        break;

    case '+':
        brightness += 20;
        if (brightness > 255)
        {
            brightness = 0;
        }
        analogWrite(GPIO30, brightness);
        break;
    }
}
