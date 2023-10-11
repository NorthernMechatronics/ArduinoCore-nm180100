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
#include "variant.h"
#include "Wire.h"

#include <am_hal_pin.h>

using namespace arduino;

static I2CPinMap PinMap = {
    .sda_pin = 6,
    .sck_pin = 5,
    .sda_pincfg = {
        .uFuncSel            = AM_HAL_PIN_6_M0SDAWIR3,
        .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
        .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
        .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
        .uIOMnum             = 0,
    },
    .sck_pincfg = {
        .uFuncSel            = AM_HAL_PIN_5_M0SCL,
        .ePullup             = AM_HAL_GPIO_PIN_PULLUP_1_5K,
        .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
        .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_OPENDRAIN,
        .uIOMnum             = 0,
    },
};

nmI2C Wire(0, &PinMap);