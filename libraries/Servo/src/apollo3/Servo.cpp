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
#include <timermap.h>
#include <wiring_analog.h>

extern "C" {
#include <am_mcu_apollo.h>
}


Servo::Servo()
{
}

uint8_t Servo::attach(int pin)
{
    return attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
    timermap_ct_available((PinName)pin, &timerSegment, &timerNumber, &outputRegister);

    if (timerSegment != -1)
    {
        this->min = min;
        this->max = max;
        this->value = DEFAULT_PULSE_WIDTH;
        this->pinNumber = pin;

        uint16_t dutycycle = this->value * 3;
        pwmWrite(pinNumber, timerSegment ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA,
                timerNumber, outputRegister, 60000, dutycycle, AM_HAL_CTIMER_HFRC_3MHZ);
    }

    return timerSegment;
}

void Servo::detach()
{
    am_hal_ctimer_stop(timerNumber,
            timerSegment ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA);
}

void Servo::write(int value)
{
    if (value < MIN_PULSE_WIDTH) {
        if (value < 0) {
            value = 0;
        } else if (value > 180) {
            value = 180;
        }

        value = map(value, 0, 180, min, max);
    }
    writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
    if (value < min)
    {
        this->value = min;
    }
    else if (value > max)
    {
        this->value = max;
    }

    this->value = value;

    uint16_t dutycycle = value * 3;
    am_hal_ctimer_stop(timerNumber,
            timerSegment ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA);
    am_hal_ctimer_clear(timerNumber,
            timerSegment ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA);
    pwmWrite(pinNumber, timerSegment ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA,
            timerNumber, outputRegister, 60000, dutycycle, AM_HAL_CTIMER_HFRC_3MHZ);
}

int Servo::read() // return the value as degrees
{
    return map(readMicroseconds(),
            min,
            max,
            0, 180);
}

int Servo::readMicroseconds()
{
    return value;
}

bool Servo::attached()
{
    if (timerSegment > -1)
        return true;

    return false;
}
