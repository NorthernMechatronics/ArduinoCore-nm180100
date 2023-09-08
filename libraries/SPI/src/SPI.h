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
#ifndef _SPI_H_
#define _SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <am_mcu_apollo.h>

#ifdef __cplusplus
}
#endif

#include <FreeRTOS.h>
#include <semphr.h>

#include "HardwareSPI.h"

namespace arduino {

struct SpiPinMap
{
    uint32_t mosi_pin;
    uint32_t miso_pin;
    uint32_t sck_pin;
    uint32_t nce_pin;
    uint32_t nce_channel;
    am_hal_gpio_pincfg_t mosi_pincfg;
    am_hal_gpio_pincfg_t miso_pincfg;
    am_hal_gpio_pincfg_t sck_pincfg;
    am_hal_gpio_pincfg_t nce_pincfg;
};

class nmSPI : public HardwareSPI
{
public:
    nmSPI(uint32_t module, SpiPinMap *pinMap);

    virtual uint8_t transfer(uint8_t data);
    virtual uint16_t transfer16(uint16_t data);
    virtual void transfer(void *buf, size_t count);

    // Transaction Functions
    virtual void usingInterrupt(int interruptNumber);
    virtual void notUsingInterrupt(int interruptNumber);
    virtual void beginTransaction(SPISettings settings);
    virtual void endTransaction(void);

    // SPI Configuration methods
    virtual void attachInterrupt();
    virtual void detachInterrupt();

    virtual void begin();
    virtual void end();

private:
    uint32_t mModule;
    am_hal_iom_config_t mConfig;
    SpiPinMap *mPinMap;
    void *mIomHandle;
    SemaphoreHandle_t mMutex;
};

}

#define SPISettings     arduino::SPISettings
#define SPIClass        arduino::nmSPI
#define SPI_MODE0       arduino::SPI_MODE0
#define SPI_MODE1       arduino::SPI_MODE1
#define SPI_MODE2       arduino::SPI_MODE2
#define SPI_MODE3       arduino::SPI_MODE3

#endif