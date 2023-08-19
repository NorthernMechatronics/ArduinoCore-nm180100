/*
  wiring_digital.c - digital input and output functions
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul
*/
#include "am_mcu_apollo.h"
#include "ArduinoAPI.h"

static am_hal_gpio_pincfg_t gpio_pincfg[AM_HAL_GPIO_MAX_PADS] = { 0 };

void pinMode(pin_size_t pin, PinMode mode)
{
    am_hal_gpio_pincfg_t pincfg;
    switch (mode)
    {
    case INPUT:
        pincfg = g_AM_HAL_GPIO_INPUT;
        break;
    case INPUT_PULLUP:
        pincfg = g_AM_HAL_GPIO_INPUT_PULLUP_1_5;
        break;
    case OUTPUT:
        pincfg = g_AM_HAL_GPIO_OUTPUT_WITH_READ;
        break;
    default:
        pincfg = g_AM_HAL_GPIO_DISABLE;
    }

    gpio_pincfg[pin] = pincfg;
    am_hal_gpio_pinconfig(pin, pincfg);
}

void digitalWrite(pin_size_t pin, PinStatus val)
{
    am_hal_gpio_state_write(pin,
            val == HIGH ?
            AM_HAL_GPIO_OUTPUT_SET :
            AM_HAL_GPIO_OUTPUT_CLEAR);
}

void digitalToggle(pin_size_t pin)
{
    am_hal_gpio_state_write(pin, AM_HAL_GPIO_OUTPUT_TOGGLE);
}

PinStatus digitalRead(pin_size_t pin)
{
    uint32_t state;

    if (memcmp(&gpio_pincfg[pin], &g_AM_HAL_GPIO_OUTPUT_WITH_READ, 4) == 0)
    {
        am_hal_gpio_state_read(pin, AM_HAL_GPIO_OUTPUT_READ, &state);
    }
    else
    {
        am_hal_gpio_state_read(pin, AM_HAL_GPIO_INPUT_READ, &state);
    }
    
    if (state)
    {
        return HIGH;
    }

	return LOW;
}