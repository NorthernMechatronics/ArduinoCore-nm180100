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
#include <stdint.h>

#include <am_mcu_apollo.h>
#include <am_util.h>

#include "ArduinoAPI.h"

#include "ctimer.h"

#define CT_MAX  (32)
#define CT_OUTSEL_MAX (4)

typedef struct ct_assignment_s {
    pin_size_t pin;
    uint32_t outsel;
} ct_assignment_t;

static ct_assignment_t ct_pinmap_tbl[CT_MAX] =
{
    {12, CT_UNUSED}, {25, CT_UNUSED}, {13, CT_UNUSED}, {26, CT_UNUSED}, {18, CT_UNUSED},
    {27, CT_UNUSED}, {19, CT_UNUSED}, {28, CT_UNUSED},  {5, CT_UNUSED}, {29, CT_UNUSED},
     {6, CT_UNUSED}, {30, CT_UNUSED}, {22, CT_UNUSED}, {31, CT_UNUSED}, {23, CT_UNUSED},
    {32, CT_UNUSED}, {42, CT_UNUSED},  {4, CT_UNUSED}, {43, CT_UNUSED},  {7, CT_UNUSED},
    {44, CT_UNUSED}, {24, CT_UNUSED}, {45, CT_UNUSED}, {33, CT_UNUSED}, {46, CT_UNUSED},
    {39, CT_UNUSED}, {47, CT_UNUSED}, {35, CT_UNUSED}, {48, CT_UNUSED}, {37, CT_UNUSED},
    {49, CT_UNUSED}, {11, CT_UNUSED}
};

static const uint32_t ct_outsel_tbl[CT_MAX][CT_OUTSEL_MAX] =
{
    {CT_OUTSEL(0,0,0), CT_OUTSEL(1,2,1), CT_OUTSEL(0,5,1), CT_OUTSEL(0,6,0)},     // CTX0:  A0OUT,  B2OUT2, A5OUT2, A6OUT
    {CT_OUTSEL(0,0,1), CT_OUTSEL(0,0,0), CT_OUTSEL(0,5,0), CT_OUTSEL(1,7,1)},     // CTX1:  A0OUT2, A0OUT,  A5OUT,  B7OUT2
    {CT_OUTSEL(1,0,0), CT_OUTSEL(1,1,1), CT_OUTSEL(1,6,1), CT_OUTSEL(0,7,0)},     // CTX2:  B0OUT,  B1OUT2, B6OUT2, A7OUT
    {CT_OUTSEL(1,0,1), CT_OUTSEL(1,0,0), CT_OUTSEL(0,1,0), CT_OUTSEL(0,6,0)},     // CTX3:  B0OUT2, B0OUT,  A1OUT,  A6OUT
    {CT_OUTSEL(0,1,0), CT_OUTSEL(0,2,1), CT_OUTSEL(0,5,1), CT_OUTSEL(1,5,0)},     // CTX4:  A1OUT,  A2OUT2, A5OUT2, B5OUT
    {CT_OUTSEL(0,1,1), CT_OUTSEL(0,1,0), CT_OUTSEL(1,6,0), CT_OUTSEL(0,7,0)},     // CTX5:  A1OUT2, A1OUT,  B6OUT,  A7OUT
    {CT_OUTSEL(1,1,0), CT_OUTSEL(0,1,0), CT_OUTSEL(1,5,1), CT_OUTSEL(1,7,0)},     // CTX6:  B1OUT,  A1OUT,  B5OUT2, B7OUT
    {CT_OUTSEL(1,1,1), CT_OUTSEL(1,1,0), CT_OUTSEL(1,5,0), CT_OUTSEL(0,7,0)},     // CTX7:  B1OUT2, B1OUT,  B5OUT,  A7OUT
    {CT_OUTSEL(0,2,0), CT_OUTSEL(0,3,1), CT_OUTSEL(0,4,1), CT_OUTSEL(1,6,0)},     // CTX8:  A2OUT,  A3OUT2, A4OUT2, B6OUT
    {CT_OUTSEL(0,2,1), CT_OUTSEL(0,2,0), CT_OUTSEL(0,4,0), CT_OUTSEL(1,0,0)},     // CTX9:  A2OUT2, A2OUT,  A4OUT,  B0OUT
    {CT_OUTSEL(1,2,0), CT_OUTSEL(1,3,1), CT_OUTSEL(1,4,1), CT_OUTSEL(0,6,0)},     // CTX10: B2OUT,  B3OUT2, B4OUT2, A6OUT
    {CT_OUTSEL(1,2,1), CT_OUTSEL(1,2,0), CT_OUTSEL(1,4,0), CT_OUTSEL(1,5,1)},     // CTX11: B2OUT2, B2OUT,  B4OUT,  B5OUT2
    {CT_OUTSEL(0,3,0), CT_OUTSEL(1,1,0), CT_OUTSEL(1,0,1), CT_OUTSEL(1,6,1)},     // CTX12: A3OUT,  B1OUT,  B0OUT2, B6OUT2
    {CT_OUTSEL(0,3,1), CT_OUTSEL(0,3,0), CT_OUTSEL(0,6,0), CT_OUTSEL(1,4,1)},     // CTX13: A3OUT2, A3OUT,  A6OUT,  B4OUT2
    {CT_OUTSEL(1,3,0), CT_OUTSEL(1,1,0), CT_OUTSEL(1,7,1), CT_OUTSEL(0,7,0)},     // CTX14: B3OUT,  B1OUT,  B7OUT2, A7OUT
    {CT_OUTSEL(1,3,1), CT_OUTSEL(1,3,0), CT_OUTSEL(0,7,0), CT_OUTSEL(0,4,1)},     // CTX15: B3OUT2, B3OUT,  A7OUT,  A4OUT2
    {CT_OUTSEL(0,4,0), CT_OUTSEL(0,0,0), CT_OUTSEL(0,0,1), CT_OUTSEL(1,3,1)},     //*CTX16: A4OUT,  A0OUT,  A0OUT2, B3OUT2
    {CT_OUTSEL(0,4,1), CT_OUTSEL(1,7,0), CT_OUTSEL(0,4,0), CT_OUTSEL(0,1,1)},     // CTX17: A4OUT2, B7OUT,  A4OUT,  A1OUT2
    {CT_OUTSEL(1,4,0), CT_OUTSEL(1,0,0), CT_OUTSEL(0,0,0), CT_OUTSEL(0,3,1)},     //*CTX18: B4OUT,  B0OUT,  A0OUT,  A3OUT2
    {CT_OUTSEL(1,4,1), CT_OUTSEL(0,2,0), CT_OUTSEL(1,4,0), CT_OUTSEL(1,1,1)},     // CTX19: B4OUT2, A2OUT,  B4OUT,  B1OUT2
    {CT_OUTSEL(0,5,0), CT_OUTSEL(0,1,0), CT_OUTSEL(0,1,1), CT_OUTSEL(1,2,1)},     //*CTX20: A5OUT,  A1OUT,  A1OUT2, B2OUT2
    {CT_OUTSEL(0,5,1), CT_OUTSEL(0,1,0), CT_OUTSEL(1,5,0), CT_OUTSEL(0,0,1)},     // CTX21: A5OUT2, A1OUT,  B5OUT,  A0OUT2
    {CT_OUTSEL(1,5,0), CT_OUTSEL(0,6,0), CT_OUTSEL(0,1,0), CT_OUTSEL(0,2,1)},     // CTX22: B5OUT,  A6OUT,  A1OUT,  A2OUT2
    {CT_OUTSEL(1,5,1), CT_OUTSEL(0,7,0), CT_OUTSEL(0,5,0), CT_OUTSEL(1,0,1)},     // CTX23: B5OUT2, A7OUT,  A5OUT,  B0OUT2
    {CT_OUTSEL(0,6,0), CT_OUTSEL(0,2,0), CT_OUTSEL(0,1,0), CT_OUTSEL(1,1,1)},     // CTX24: A6OUT,  A2OUT,  A1OUT,  B1OUT2
    {CT_OUTSEL(1,4,1), CT_OUTSEL(1,2,0), CT_OUTSEL(0,6,0), CT_OUTSEL(0,2,1)},     //*CTX25: B4OUT2, B2OUT,  A6OUT,  A2OUT2
    {CT_OUTSEL(1,6,0), CT_OUTSEL(1,2,0), CT_OUTSEL(0,5,0), CT_OUTSEL(0,1,1)},     //*CTX26: B6OUT,  B2OUT,  A5OUT,  A1OUT2
    {CT_OUTSEL(1,6,1), CT_OUTSEL(0,1,0), CT_OUTSEL(1,6,0), CT_OUTSEL(1,2,1)},     // CTX27: B6OUT2, A1OUT,  B6OUT,  B2OUT2
    {CT_OUTSEL(0,7,0), CT_OUTSEL(0,3,0), CT_OUTSEL(0,5,1), CT_OUTSEL(1,0,1)},     // CTX28: A7OUT,  A3OUT,  A5OUT2, B0OUT2
    {CT_OUTSEL(1,5,1), CT_OUTSEL(0,1,0), CT_OUTSEL(0,7,0), CT_OUTSEL(0,3,1)},     // CTX29: B5OUT2, A1OUT,  A7OUT,  A3OUT2
    {CT_OUTSEL(1,7,0), CT_OUTSEL(1,3,0), CT_OUTSEL(0,4,1), CT_OUTSEL(0,0,1)},     // CTX30: B7OUT,  B3OUT,  A4OUT2, A0OUT2
    {CT_OUTSEL(1,7,1), CT_OUTSEL(0,6,0), CT_OUTSEL(1,7,0), CT_OUTSEL(1,3,1)},     // CTX31: B7OUT2, A6OUT,  B7OUT,  B3OUT2
};

uint32_t ct_find_timer(pin_size_t pinNumber)
{
    for (int i = 0; i < CT_MAX; i++)
    {
        if (ct_pinmap_tbl[i].pin == pinNumber)
        {
            return i;
        }
    }

    return CT_ERROR;
}

void ct_assignment_set_by_timer(uint32_t timer, uint32_t outsel)
{
    ct_pinmap_tbl[timer].outsel = outsel;
}

void ct_assignment_unset_by_timer(uint32_t timer)
{
    ct_pinmap_tbl[timer].outsel = CT_UNUSED;
}

void ct_assignment_set_by_pin(pin_size_t pinNumber, uint32_t outsel)
{
    uint32_t timer = ct_find_timer(pinNumber);
    ct_pinmap_tbl[timer].outsel = outsel;
}

void ct_assignment_unset_by_pin(pin_size_t pinNumber)
{
    uint32_t timer = ct_find_timer(pinNumber);
    ct_pinmap_tbl[timer].outsel = CT_UNUSED;
}

pin_size_t ct_assignment_get_by_outsel(uint32_t outsel)
{
    for (int i = 0; i < CT_MAX; i++)
    {
        if (CT_OUTSEL_CMP(ct_pinmap_tbl[i].outsel, outsel))
        {
            return ct_pinmap_tbl[i].pin;
        }
    }

    return CT_UNUSED;
}

uint32_t ct_assignment_get_by_pin(pin_size_t pinNumber)
{
    uint32_t timer = ct_find_timer(pinNumber);
    return ct_pinmap_tbl[timer].outsel;
}

uint32_t ct_assignment_get_by_timer(uint32_t timer)
{
    return ct_pinmap_tbl[timer].outsel;
}

uint32_t ct_assign(pin_size_t pinNumber)
{
    uint32_t timer = ct_find_timer(pinNumber);
    if (timer == CT_ERROR)
    {
        return CT_ERROR;
    }

    for (int i = 0; i < CT_OUTSEL_MAX; i++)
    {
        uint32_t outsel = ct_outsel_tbl[timer][i];
        pin_size_t pin = ct_assignment_get_by_outsel(outsel);
        if (pin == CT_UNUSED)
        {
            ct_assignment_set_by_timer(timer, outsel);
            return outsel;
        }
    }

    return CT_ERROR;
}

void ct_start(uint32_t outsel)
{
    uint8_t seg, num;
    seg = CT_OUTSEL_SEG(outsel);
    num = CT_OUTSEL_NUM(outsel);
    am_hal_ctimer_start(num, seg);
}

void ct_stop(uint32_t outsel)
{
    uint8_t seg, num;
    seg = CT_OUTSEL_SEG(outsel);
    num = CT_OUTSEL_NUM(outsel);
    am_hal_ctimer_stop(num, seg);
}