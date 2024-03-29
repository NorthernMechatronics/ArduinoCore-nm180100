/*
 *  BSD 3-Clause License
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

#include <stdlib.h>
#include <am_mcu_apollo.h>

#include <FreeRTOS.h>
#include <task.h>

#include "ArduinoAPI.h"

extern "C" uint32_t clockFrequency(void);

static void pulseCallback(void *parameter)
{
    BaseType_t context_switch;
    TaskHandle_t handle = (TaskHandle_t)parameter;
    xTaskNotifyFromISR(handle, 0, eNoAction, &context_switch);
    portYIELD_FROM_ISR(context_switch);
}

unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout)
{
    unsigned long duration_us = 0;
    uint32_t tick_start, tick_stop, tick_current;
    uint32_t tick_timeout, tick_end, cpu_frequency;

    am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_INPUT);

    cpu_frequency = clockFrequency();
    tick_timeout = (timeout / 1000000.0) * cpu_frequency;

    tick_start = am_hal_stimer_counter_get();
    tick_end = tick_start + tick_timeout;
    while (am_hal_gpio_input_read(pin) == state)
    {
        tick_current = am_hal_stimer_counter_get();
        if (tick_current > tick_end)
        {
            am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);
            return 0;
        }
    }

    while (am_hal_gpio_input_read(pin) != state)
    {
        tick_current = am_hal_stimer_counter_get();
        if (tick_current > tick_end)
        {
            am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);
            return 0;
        }
    }

    tick_start = am_hal_stimer_counter_get();
    tick_end = tick_start + tick_timeout;
    while (am_hal_gpio_input_read(pin) == state)
    {
        tick_current = am_hal_stimer_counter_get();
        if (tick_current > tick_end)
        {
            am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);
            return 0;
        }
    }
    tick_stop = am_hal_stimer_counter_get();
    am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);

    duration_us = (tick_stop - tick_start) * 1000000 / cpu_frequency;

    return duration_us;
}

unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout)
{
    TaskHandle_t current_task;
    unsigned long duration_us = 0;
    uint32_t tick_start, tick_stop, tick_current;
    uint32_t tick_timeout, tick_end, cpu_frequency;

    am_hal_gpio_pincfg_t gpio_pincfg = g_AM_HAL_GPIO_INPUT;
    AM_HAL_GPIO_MASKCREATE(sGpioMask);
    AM_HAL_GPIO_MASKBIT(psGpioMask, pin);

    cpu_frequency = clockFrequency();
    tick_timeout = (timeout / 1000000.0) * cpu_frequency;
    current_task = xTaskGetCurrentTaskHandle();

    gpio_pincfg.eIntDir = AM_HAL_GPIO_PIN_INTDIR_BOTH;
    am_hal_gpio_pinconfig(pin, gpio_pincfg);
    am_hal_gpio_interrupt_register_adv(pin, pulseCallback, current_task);
    am_hal_gpio_interrupt_clear(psGpioMask);
    am_hal_gpio_interrupt_enable(psGpioMask);

    if (am_hal_gpio_input_read(pin) == state)
    {
        if (xTaskNotifyWait(0, 1, NULL, pdMS_TO_TICKS(timeout / 1000)) == pdFALSE)
        {
            am_hal_gpio_interrupt_disable(psGpioMask);
            am_hal_gpio_interrupt_clear(psGpioMask);
            am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);
            return 0;
        }
    }

    if (am_hal_gpio_input_read(pin) != state)
    {
        if (xTaskNotifyWait(0, 1, NULL, pdMS_TO_TICKS(timeout / 1000)) == pdFALSE)
        {
            am_hal_gpio_interrupt_disable(psGpioMask);
            am_hal_gpio_interrupt_clear(psGpioMask);
            am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);
            return 0;
        }
    }

    tick_start = am_hal_stimer_counter_get();
    tick_end = tick_start + tick_timeout;
    if (am_hal_gpio_input_read(pin) == state)
    {
        if (xTaskNotifyWait(0, 1, NULL, pdMS_TO_TICKS(timeout / 1000)) == pdFALSE)
        {
            am_hal_gpio_interrupt_disable(psGpioMask);
            am_hal_gpio_interrupt_clear(psGpioMask);
            am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);
            return 0;
        }
    }
    tick_stop = am_hal_stimer_counter_get();
    am_hal_gpio_interrupt_disable(psGpioMask);
    am_hal_gpio_interrupt_clear(psGpioMask);
    am_hal_gpio_pinconfig(pin, g_AM_HAL_GPIO_DISABLE);

    duration_us = (tick_stop - tick_start) * 1000000 / cpu_frequency;

    return duration_us;
}