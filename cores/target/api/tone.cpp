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

#include <stdlib.h>

extern "C"
{
#include <am_mcu_apollo.h>
}

#include "ArduinoAPI.h"
#include "ctimer.h"

static uint32_t tone_output_selection;
static uint64_t tone_cycle_count;

void tone_complete_callback(void)
{
    if (--tone_cycle_count == 0)
    {
        ct_stop(tone_output_selection);
    }
}

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    uint32_t seg, num, reg;

    uint32_t timer = ct_find_timer(_pin);
    if (timer == CT_ERROR)
    {
        return;
    }

    uint32_t outsel = ct_assignment_get_by_timer(timer);
    if (outsel == CT_UNUSED)
    {
        outsel = ct_assign(_pin);
    }
    ct_stop(outsel);

    seg = CT_OUTSEL_SEG(outsel);
    num = CT_OUTSEL_NUM(outsel);
    reg = CT_OUTSEL_REG(outsel);

    uint32_t period, duty_cycle;
    period = 3e6 / frequency;
    duty_cycle = period >> 1;

    uint32_t config_flag = AM_HAL_CTIMER_FN_PWM_REPEAT | AM_HAL_CTIMER_HFRC_3MHZ | AM_HAL_CTIMER_PIN_INVERT;
    uint32_t interrupt_mask = 0;
    if (duration > 0)
    {
        tone_cycle_count = (uint64_t)duration * (uint64_t)frequency / 1000;
        config_flag |= AM_HAL_CTIMER_INT_ENABLE;
        interrupt_mask = 1 << (num << 1);
        if (seg == 1)
        {
            interrupt_mask <<= 1;
        }
        tone_output_selection = outsel;
        am_hal_ctimer_int_register(interrupt_mask, tone_complete_callback);
        am_hal_ctimer_int_enable(interrupt_mask);
    }

    uint32_t ctimer_segment = seg ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA;
    am_hal_ctimer_output_config(
        num, ctimer_segment, _pin, AM_HAL_CTIMER_OUTPUT_NORMAL, AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA);
    am_hal_ctimer_config_single(num, ctimer_segment, config_flag);

    am_hal_ctimer_period_set(num, ctimer_segment, period, duty_cycle);
    am_hal_ctimer_aux_period_set(num, ctimer_segment, period, duty_cycle);

    am_hal_ctimer_start(num, ctimer_segment);
}

void noTone(uint8_t _pin)
{
    uint32_t seg, num, reg;

    uint32_t timer = ct_find_timer(_pin);
    if (timer == CT_ERROR)
    {
        return;
    }

    uint32_t outsel = ct_assignment_get_by_timer(timer);
    if (outsel != CT_UNUSED)
    {
        ct_stop(outsel);
    }
}