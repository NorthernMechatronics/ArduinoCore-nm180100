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

#include "ArduinoAPI.h"
#include "ctimer.h"

static uint32_t pwm_period = 255;

void analogWrite(pin_size_t pinNumber, int value)
{
    uint32_t seg, num, reg;
    uint32_t timer = ct_find_timer(pinNumber);
    uint32_t outsel = ct_assignment_get_by_timer(timer);
    if (outsel == CT_UNUSED)
    {
        outsel = ct_assign(pinNumber);
    }
    seg = CT_OUTSEL_SEG(outsel);
    num = CT_OUTSEL_NUM(outsel);
    reg = CT_OUTSEL_REG(outsel);

    am_hal_ctimer_stop(num, seg);

    if (value == 0)
    {
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, LOW);
    }
    else if (value == pwm_period)
    {
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, HIGH);
    }
    else
    {
        if (seg == 0)
        {
            seg = AM_HAL_CTIMER_TIMERA;
        }
        else
        {
            seg = AM_HAL_CTIMER_TIMERB;
        }

        am_hal_ctimer_output_config(
            num,
            seg,
            pinNumber,
            AM_HAL_CTIMER_OUTPUT_NORMAL,
            AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA
        );
        am_hal_ctimer_config_single(
            num,
            seg,
            (AM_HAL_CTIMER_FN_PWM_REPEAT | AM_HAL_CTIMER_HFRC_12KHZ)
        );
        am_hal_ctimer_period_set(num, seg, pwm_period, value);
        am_hal_ctimer_aux_period_set(num, seg, pwm_period, value);
        am_hal_ctimer_start(num, seg);
    }
}

void analogWriteResolution(uint32_t resolution)
{
    pwm_period = (1 << resolution) - 1;
}