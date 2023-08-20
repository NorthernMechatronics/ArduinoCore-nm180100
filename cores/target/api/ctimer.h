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
#ifndef _CTIMER_H_
#define _CTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CT_ERROR  (0xFF)
#define CT_UNUSED (0xFF)

#define CT_OUTSEL(seg, num, reg) ((reg << 4) | (seg << 3) | (num << 0))
#define CT_OUTSEL_NUM(val)       (val & (0x7 << 0))
#define CT_OUTSEL_SEG(val)       ((val & (0x1 << 3)) >> 3)
#define CT_OUTSEL_REG(val)       ((val & (0x1 << 4)) >> 4)
#define CT_OUTSEL_CMP(val1, val2) ((val1 & ~(0x1 << 4)) == (val2 & ~(0x1 << 4)))

uint32_t ct_find_timer(pin_size_t pinNumber);
void ct_assignment_set_by_timer(uint32_t timer, uint32_t outsel);
void ct_assignment_unset_by_timer(uint32_t timer);
void ct_assignment_set_by_pin(pin_size_t pinNumber, uint32_t outsel);
void ct_assignment_unset_by_pin(pin_size_t pinNumber);
pin_size_t ct_assignment_get_by_outsel(uint32_t outsel);
uint32_t ct_assignment_get_by_pin(pin_size_t pinNumber);
uint32_t ct_assignment_get_by_timer(uint32_t timer);
uint32_t ct_assign(pin_size_t pinNumber);
void ct_start(uint32_t outsel);
void ct_stop(uint32_t outsel);

#ifdef __cplusplus
}
#endif

#endif