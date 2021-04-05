/*
  Copyright (c) 2017 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include <Arduino.h>
#include <Servo.h>

static servo_t servos[MAX_SERVOS];
static volatile int8_t timerChannel = -1;
static uint8_t ServoCount = 0;
static volatile uint32_t CumulativeCountSinceRefresh = 0;

Servo::Servo() : min(MIN_PULSE_WIDTH), max(MAX_PULSE_WIDTH)
{
    if (ServoCount < MAX_SERVOS) {
        servoIndex = ServoCount++;
        servos[servoIndex].ticks = DEFAULT_PULSE_WIDTH;
    } else {
        servoIndex = INVALID_SERVO;
    }
}

uint8_t Servo::attach(int pin)
{
    return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
    if (servoIndex < MAX_SERVOS) {
        servos[servoIndex].Pin.pinNumber = pin;
        this->min = (MIN_PULSE_WIDTH - min) / 4;
        this->max = (MAX_PULSE_WIDTH - max) / 4;
        servos[servoIndex].Pin.isActive = true;
    }

    return servoIndex;
}

void Servo::detach() { servos[servoIndex].Pin.isActive = false; }

void Servo::write(int value)
{
    // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if (value < MIN_PULSE_WIDTH) {
        if (value < 0) {
            value = 0;
        } else if (value > 180) {
            value = 180;
        }

        value = map(value, 0, 180, (MIN_PULSE_WIDTH - min * 4),
                    (MAX_PULSE_WIDTH - max * 4));
    }
    writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
    // calculate and store the values for the given channel
    byte channel = this->servoIndex;
    if ((channel < MAX_SERVOS)) { // ensure channel is valid
        if (value <
            (MIN_PULSE_WIDTH - min * 4)) { // ensure pulse width is valid
            value = (MIN_PULSE_WIDTH - min * 4);
        } else if (value > (MAX_PULSE_WIDTH - max * 4)) {
            value = (MAX_PULSE_WIDTH - max * 4);
        }

        servos[channel].ticks = value;
    }
}

int Servo::read() // return the value as degrees
{
    return map(readMicroseconds() + 1,
            (MIN_PULSE_WIDTH - min * 4),
            (MAX_PULSE_WIDTH - max * 4),
            0, 180);
}

int Servo::readMicroseconds()
{
    unsigned int pulsewidth;
    if (this->servoIndex != INVALID_SERVO) {
        pulsewidth = servos[this->servoIndex].ticks;
    } else {
        pulsewidth = 0;
    }

    return pulsewidth;
}

bool Servo::attached() { return servos[this->servoIndex].Pin.isActive; }
