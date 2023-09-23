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
#include <am_util.h>

#include <FreeRTOS.h>
#include <task.h>

#include "ArduinoAPI.h"

uint32_t clockFrequency(void);

void delay(unsigned long ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void delayMicroseconds(unsigned int us)
{
    am_util_delay_us(us);
}

unsigned long millis(void)
{
    float_t current_ticks = am_hal_stimer_counter_get();
    float_t scaled_ticks = current_ticks * 1000;
    float_t frequency = clockFrequency();
    float_t elapsed_ms = scaled_ticks / frequency;

    while (elapsed_ms > UINT32_MAX)
    {
        elapsed_ms -= UINT32_MAX;
    }

    return (unsigned long)elapsed_ms;
}

unsigned long micros(void)
{
    float_t frequency = clockFrequency();
    float_t current_ticks = am_hal_stimer_counter_get();
    float_t scaled_ticks = current_ticks * 1000000;
    float_t elapsed_us = scaled_ticks / frequency;

    while (elapsed_us > UINT32_MAX)
    {
        elapsed_us -= UINT32_MAX;
    }

    return (unsigned long)elapsed_us;
}

void yield(void)
{
    taskYIELD();
}

uint32_t clockFrequency(void)
{
    uint32_t ui32CurrConfig = CTIMER->STCFG;
    uint32_t ui32Clksel = _FLD2VAL(CTIMER_STCFG_CLKSEL, ui32CurrConfig);
    uint32_t ui32Frequency = 0;

    switch(ui32Clksel)
    {
    case CTIMER_STCFG_CLKSEL_HFRC_DIV16:
        ui32Frequency = 3000000;
        break;

    case CTIMER_STCFG_CLKSEL_HFRC_DIV256:
        ui32Frequency = 187500;
        break;

    case CTIMER_STCFG_CLKSEL_XTAL_DIV1:
        ui32Frequency = 32768;
        break;

    case CTIMER_STCFG_CLKSEL_XTAL_DIV2:
        ui32Frequency = 16384;
        break;

    case CTIMER_STCFG_CLKSEL_XTAL_DIV32:
        ui32Frequency = 1024;
        break;

    case CTIMER_STCFG_CLKSEL_LFRC_DIV1:
        ui32Frequency = 1024;
        break;

    default:
        break;
    }

    return ui32Frequency;

}