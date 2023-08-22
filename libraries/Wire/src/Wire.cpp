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

#include "Wire.h"
#include "ArduinoAPI.h"

using namespace arduino;

nmI2C::nmI2C(uint32_t module, I2CPinMap *pinMap)
{
}

void nmI2C::begin()
{
}

void nmI2C::begin(uint8_t address)
{
}

void nmI2C::end()
{
}

void nmI2C::setClock(uint32_t freq)
{
}

void nmI2C::beginTransmission(uint8_t address)
{
}

uint8_t nmI2C::endTransmission(bool stopBit)
{
    return 0;
}

uint8_t nmI2C::endTransmission(void)
{
    return 0;
}

size_t nmI2C::requestFrom(uint8_t address, size_t len, bool stopBit)
{
    return 0;
}

size_t nmI2C::requestFrom(uint8_t address, size_t len)
{
    return 0;
}

void nmI2C::onReceive(void (*)(int))
{
}

void nmI2C::onRequest(void (*)(void))
{
}