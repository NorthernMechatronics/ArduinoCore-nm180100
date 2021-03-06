/*
  Copyright (c) 2011 Arduino.  All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "am_mcu_apollo.h"

#ifdef __cplusplus
}
#endif

#include "Arduino.h"

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * ATTENTION:
 * This function relies on micros() so cannot be used in noInterrupt() context
 */
unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout)
{
  uint32_t bit;
  am_hal_gpio_state_read(pin, AM_HAL_GPIO_INPUT_READ, &bit);

  uint32_t stateMask = (state ? bit : 0);
  uint32_t startMicros = micros();


  // wait for any previous pulse to end
  while (am_hal_gpio_input_read(pin) == stateMask) {
    if (micros() - startMicros > timeout) {
      return 0;
    }
  }

  // wait for the pulse to start
  while (am_hal_gpio_input_read(pin) != stateMask) {
    if (micros() - startMicros > timeout) {
      return 0;
    }
  }

  uint32_t start = micros();
  // wait for the pulse to stop
  while (am_hal_gpio_input_read(pin) == stateMask) {
    if (micros() - startMicros > timeout) {
      return 0;
    }
  }
  return (micros() - start);
}

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * ATTENTION:
 * This function relies on micros() so cannot be used in noInterrupt() context
 */
unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout)
{
  return pulseIn(pin, state, timeout);
}
