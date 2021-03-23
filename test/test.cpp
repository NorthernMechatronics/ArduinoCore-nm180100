#include <Arduino.h>

unsigned int counter = 0;
int brightness = 210;
int incoming = 0;
unsigned current = 0;
unsigned previous = 0;
unsigned period = 1000;

void setup()
{
    //pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    tone(GPIO05, 100, 5000);
    Serial.begin(115200);

    current = millis();
    previous = current;
}

void loop()
{
    current = millis();
    if ((current - previous) > period)
    {
        previous = current;
        digitalToggle(LED4);
        Serial.println(counter++);
    }

    incoming = Serial.read();
    if(incoming == 's')
    {
        int voltage_code = analogRead(GPIO29);
        // say what you got:
        Serial.print("Analog Code: ");
        Serial.println(voltage_code, DEC);
    }
    else if (incoming == 'i')
    {
        brightness += 20;
        if (brightness > 255)
        {
            brightness = 0;
        }
        analogWrite(LED1, brightness);
    }
}

