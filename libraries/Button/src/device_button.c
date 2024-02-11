/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Northern Mechatronics, Inc.
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
#include <stdint.h>

#include <am_mcu_apollo.h>

#include "device_button.h"

static uint32_t device_button_gpio = 16;
static am_hal_gpio_pincfg_t device_button_pincfg = {
    .uFuncSel            = 3,
    .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA,
    .eGPInput            = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI
};

void device_button_initialize(uint32_t button_gpio)
{
    device_button_gpio = button_gpio;

    am_hal_gpio_pinconfig(device_button_gpio, device_button_pincfg);

    am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(device_button_gpio));
    am_hal_gpio_interrupt_enable(AM_HAL_GPIO_BIT(device_button_gpio));
}

void device_button_uninitialize()
{
    am_hal_gpio_interrupt_disable(AM_HAL_GPIO_BIT(device_button_gpio));
    am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(device_button_gpio));

    am_hal_gpio_pinconfig(device_button_gpio, g_AM_HAL_GPIO_DISABLE);
}

void device_button_interrupt_disable()
{
    am_hal_gpio_interrupt_disable(AM_HAL_GPIO_BIT(device_button_gpio));
    am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(device_button_gpio));
}

void device_button_interrupt_enable()
{
    am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(device_button_gpio));
    am_hal_gpio_interrupt_enable(AM_HAL_GPIO_BIT(device_button_gpio));
}

void device_button_interrupt_register(am_hal_gpio_handler_t pfnHandler)
{
    am_hal_gpio_interrupt_register(device_button_gpio, pfnHandler);
}

button_state_e device_button_read_state()
{
    uint32_t state;

    am_hal_gpio_state_read(device_button_gpio, AM_HAL_GPIO_INPUT_READ, &state);

    return (state == 0) ? BUTTON_PRESSED : BUTTON_UNPRESSED;
}
