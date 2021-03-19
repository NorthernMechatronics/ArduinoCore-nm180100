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
#ifdef __cplusplus
extern "C" {
#endif

#include "am_mcu_apollo.h"

#ifdef __cplusplus
}
#endif

#define ARDUINO_MAIN
#include "Common.h"

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
        pincfg = g_AM_HAL_GPIO_OUTPUT;
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

    am_hal_gpio_state_read(pin, AM_HAL_GPIO_INPUT_READ, &state);
    
    if (state)
        return HIGH;

	return LOW;
}

static uint8_t configInterrupt(pin_size_t interruptNumber, PinStatus mode)
{
    am_hal_gpio_intdir_e intdir;

    switch (mode)
    {
    case FALLING:
        intdir = AM_HAL_GPIO_PIN_INTDIR_HI2LO;
        break;

    case RISING:
        intdir = AM_HAL_GPIO_PIN_INTDIR_LO2HI;
        break;

    case CHANGE:
        intdir = AM_HAL_GPIO_PIN_INTDIR_BOTH;
        break;

    case LOW:
    case HIGH:
    default:
        intdir = AM_HAL_GPIO_PIN_INTDIR_NONE;
        break;
    }

    gpio_pincfg[interruptNumber].eIntDir = intdir;
    am_hal_gpio_pinconfig(interruptNumber, gpio_pincfg[interruptNumber]);

    if (intdir == AM_HAL_GPIO_PIN_INTDIR_NONE)
    {
        am_hal_gpio_interrupt_disable(AM_HAL_GPIO_BIT(interruptNumber));
        am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(interruptNumber));

        return 0;
    }
    else
    {
        am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(interruptNumber));
        am_hal_gpio_interrupt_enable(AM_HAL_GPIO_BIT(interruptNumber));

        NVIC_EnableIRQ(GPIO_IRQn);

        return 1;
    }

    return 0;
}

void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode)
{
    if (configInterrupt(interruptNumber, mode))
    {
        am_hal_gpio_interrupt_register(interruptNumber, callback);
    }
}

void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param)
{
    if (configInterrupt(interruptNumber, mode))
    {
        am_hal_gpio_interrupt_register_adv(interruptNumber, callback, param);
    }
}

void detachInterrupt(pin_size_t interruptNumber)
{
    am_hal_gpio_interrupt_disable(AM_HAL_GPIO_BIT(interruptNumber));
    am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(interruptNumber));
    am_hal_gpio_interrupt_register(interruptNumber, NULL);
}