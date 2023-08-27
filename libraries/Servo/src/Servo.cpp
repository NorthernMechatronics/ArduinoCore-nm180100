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
extern "C"
{
#include <am_mcu_apollo.h>
}

#include <ArduinoAPI.h>
#include "ctimer.h"
#include "Servo.h"

#define SERVO_CLK_FREQUENCY        (3e6)
#define SERVO_PERIOD             (60000)
#define SERVO_US_TO_TICKS_SCALAR     (3)

Servo::Servo()
  : pin(0),
    timer_number(0),
    timer_segment(0),
    timer_output(0),
    min_ang(0),
    max_ang(0),
    outsel(CT_UNUSED)
{
}

uint8_t Servo::attach(int pin)
{
    return attach(pin, -30, 30);
}

uint8_t Servo::attach(int pin, int min_ang, int max_ang)
{
    uint32_t timer = ct_find_timer(pin);
    if (timer == CT_ERROR)
    {
        return false;
    }

    this->outsel = ct_assignment_get_by_timer(timer);
    if (this->outsel == CT_UNUSED)
    {
        this->outsel = ct_assign(pin);
    }
    else
    {
        return false;
    }

    this->timer_number  = CT_OUTSEL_NUM(this->outsel);
    this->timer_segment = CT_OUTSEL_SEG(this->outsel) ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA;
    this->timer_output  = CT_OUTSEL_REG(this->outsel) ? AM_HAL_CTIMER_OUTPUT_SECONDARY : AM_HAL_CTIMER_OUTPUT_NORMAL;

    this->min_ang = min_ang;
    this->max_ang = max_ang;

    this->pin = pin;
    return true;
}

void Servo::detach()
{
    if (this->outsel != CT_UNUSED)
    {
        ct_stop(this->outsel);
    }
}

void Servo::write(int value)
{
    if (value < MIN_PULSE_WIDTH)
    {
        if (value < this->min_ang)
            value = this->min_ang;
        else if (value > this->max_ang)
            value = this->max_ang;

        value = map(value, this->min_ang, this->max_ang, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    }
    writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
    if (value < MIN_PULSE_WIDTH)
    {
        value = MIN_PULSE_WIDTH;
    }
    else if (value > MAX_PULSE_WIDTH)
    {
        value = MAX_PULSE_WIDTH;
    }
 
    this->duty_cycle = value * SERVO_US_TO_TICKS_SCALAR;

    uint32_t config_flag = AM_HAL_CTIMER_FN_PWM_REPEAT | AM_HAL_CTIMER_HFRC_3MHZ;

    am_hal_ctimer_stop(this->timer_number, this->timer_segment);
    am_hal_ctimer_clear(this->timer_number, this->timer_segment);
    am_hal_ctimer_output_config(
        this->timer_number, this->timer_segment,
        this->pin,
        AM_HAL_CTIMER_OUTPUT_NORMAL,
        AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA);
    am_hal_ctimer_config_single(this->timer_number, this->timer_segment, config_flag);

    if (timer_output == AM_HAL_CTIMER_OUTPUT_SECONDARY)
    {
        am_hal_ctimer_period_set(this->timer_number, this->timer_segment, SERVO_PERIOD, this->duty_cycle);
        am_hal_ctimer_aux_period_set(this->timer_number, this->timer_segment, SERVO_PERIOD, this->duty_cycle);
    }
    else
    {
        am_hal_ctimer_period_set(this->timer_number, this->timer_segment, SERVO_PERIOD, this->duty_cycle);
    }
    am_hal_ctimer_start(this->timer_number, this->timer_segment);
}

int Servo::read()
{
    return map(readMicroseconds(), MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, 0, 180);
}

int Servo::readMicroseconds()
{
    return this->duty_cycle / SERVO_US_TO_TICKS_SCALAR;
}

bool Servo::attached()
{
    if (this->outsel != CT_UNUSED)
    {
        return true;
    }

    return false;
}