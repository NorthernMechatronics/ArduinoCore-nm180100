/*
  Servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
  Copyright (c) 2009 Michael Margolis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/* 
  A servo is activated by creating an instance of the Servo class passing 
  the desired pin to the attach() method.
  The servos are pulsed in the background using the value most recently 
  written using the write() method.

  Note that analogWrite of PWM on pins associated with the timer are 
  disabled when the first servo is attached.
  Timers are seized as needed in groups of 12 servos - 24 servos use two 
  timers, 48 servos will use four.
  The sequence used to seize timers is defined in timers.h

  The methods are:

    Servo - Class for manipulating servo motors connected to Arduino pins.

    attach(pin )  - Attaches a servo motor to an I/O pin.
    attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
    default min is 544, max is 2400  
 
    write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
    writeMicroseconds() - Sets the servo pulse width in microseconds 
    read()      - Gets the last written servo pulse width as an angle between 0 and 180. 
    readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
    attached()  - Returns true if there is a servo attached. 
    detach()    - Stops an attached servos from pulsing its I/O pin. 
 */

#ifndef Servo_h
#define Servo_h

#include <inttypes.h>

/* 
 * Defines for 16 bit timers used with Servo library 
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the current board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 */

#define Servo_VERSION 2 // software version of this library

#define MIN_PULSE_WIDTH     400   // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH     2600  // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH 1500  // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000 // minimum time to refresh servos in microseconds

class Servo
{
public:
    Servo();
    uint8_t attach(int pin); 
    uint8_t attach(int pin, int min_ang, int max_ang);
    void detach();
    void write(int value); // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds
    void writeMicroseconds(int value); // Write pulse width in microseconds
    int read(); // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds(); // returns current pulse width in microseconds for this servo (was read_us() in first release)
    bool attached(); // return true if this servo is attached, otherwise false
private:
    uint32_t pin;
    uint32_t outsel;
    uint32_t timer_number;
    uint32_t timer_segment;
    uint32_t timer_output;
    uint32_t duty_cycle;
    int8_t min_ang;
    int8_t max_ang;
};

#endif
