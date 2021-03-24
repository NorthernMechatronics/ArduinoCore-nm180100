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

Servo::Servo()
{
}

uint8_t Servo::attach(int pin, int value)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, value);
}

uint8_t Servo::attach(int pin, int min, int max, int value)
{
  return 0;
}

void Servo::detach()
{
}

void Servo::write(int value)
{
}

void Servo::writeMicroseconds(int value)
{
}

int Servo::read() // return the value as degrees
{
    return 0;
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;

  return pulsewidth;
}

bool Servo::attached()
{
  return false;
}
