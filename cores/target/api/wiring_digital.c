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