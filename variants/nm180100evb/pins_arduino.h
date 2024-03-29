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
#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#include <stdint.h>

#define SCL (8)
#define SDA (9)

#define D0  (23)
#define D1  (22)
#define D2  (26)
#define D3  (49)
#define D4  (48)
#define D5  (17)
#define D6  (46)
#define D7  (45)
#define D8  (41)
#define D9  (4)
#define D10 (20)
#define D11 (7)
#define D12 (6)
#define D13 (5)
#define D14 (12)
#define D15 (3)
#define D16 (37)
#define D17 (38)
#define D18 (24)
#define D19 (21)
#define D20 (27)
#define D21 (25)

#define A0  (13)
#define A1  (29)
#define A2  (11)
#define A3  (31)
#define A4  (32)
#define A5  (33)
#define A6  (34)
#define A7  (35)

static const uint8_t MOSI = 7;
static const uint8_t MISO = 6;
static const uint8_t SCK  = 5;
static const uint8_t SS   = 4;

#endif