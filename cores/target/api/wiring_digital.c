/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2023, Northern Mechatronics, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <am_mcu_apollo.h>

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

static uint32_t configInterrupt(pin_size_t pin, PinStatus mode)
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

    default:
        intdir = AM_HAL_GPIO_PIN_INTDIR_NONE;
        break;
    }

    gpio_pincfg[pin].eIntDir = intdir;
    am_hal_gpio_pinconfig(pin, gpio_pincfg[pin]);
    
    AM_HAL_GPIO_MASKCREATE(GpioIntMask);
    AM_HAL_GPIO_MASKBIT(pGpioIntMask, pin);
    if (intdir == AM_HAL_GPIO_PIN_INTDIR_NONE)
    {
        am_hal_gpio_interrupt_disable(pGpioIntMask);
        am_hal_gpio_interrupt_clear(pGpioIntMask);
        return AM_HAL_STATUS_FAIL;
    }

    am_hal_gpio_interrupt_clear(pGpioIntMask);
    am_hal_gpio_interrupt_enable(pGpioIntMask);

    return AM_HAL_STATUS_SUCCESS;
}

void attachInterrupt(pin_size_t pin, voidFuncPtr callback, PinStatus mode)
{
    if (configInterrupt(pin, mode) == AM_HAL_STATUS_SUCCESS)
    {
        am_hal_gpio_interrupt_register(pin, callback);
    }
}

void attachInterruptParam(pin_size_t pin, voidFuncPtrParam callback, PinStatus mode, void* param)
{
    if (configInterrupt(pin, mode) == AM_HAL_STATUS_SUCCESS)
    {
        am_hal_gpio_interrupt_register_adv(pin, callback, param);
    }
}

void detachInterrupt(pin_size_t pin)
{
    AM_HAL_GPIO_MASKCREATE(GpioIntMask);
    AM_HAL_GPIO_MASKBIT(pGpioIntMask, pin);
    am_hal_gpio_interrupt_disable(pGpioIntMask);
    am_hal_gpio_interrupt_clear(pGpioIntMask);
    am_hal_gpio_interrupt_register(pin, NULL);
}