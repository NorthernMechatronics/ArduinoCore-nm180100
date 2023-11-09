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
#include "SPI_pinmap.h"
#include "SPI.h"

using namespace arduino;

static SpiPinMap PinMap = {
    .mosi_pin = 28,
    .miso_pin = 25,
    .sck_pin = 27,
    .nce_pin = 15,
    .mosi_pincfg = {
        .uFuncSel            = AM_HAL_PIN_28_M2MOSI,
        .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
        .uIOMnum             = 2
    },
    .miso_pincfg = {
        .uFuncSel            = AM_HAL_PIN_25_M2MISO,
        .uIOMnum             = 2
    },
    .sck_pincfg = {
        .uFuncSel            = AM_HAL_PIN_27_M2SCK,
        .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
        .uIOMnum             = 2
    },
    .nce_pincfg = {
        .uFuncSel            = AM_HAL_PIN_15_NCE15,
        .eDriveStrength      = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
        .eGPOutcfg           = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
        .eGPInput            = AM_HAL_GPIO_PIN_INPUT_NONE,
        .eIntDir             = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
        .uIOMnum             = 2,
        .uNCE                = 3,
        .eCEpol              = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
    },
};

nmSPI SPI2(2, &PinMap);