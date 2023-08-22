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

#include "SPI.h"
#include "ArduinoAPI.h"

using namespace arduino;

nmSPI::nmSPI(uint32_t module, SpiPinMap *pinMap) : mModule(module), mPinMap(pinMap)
{
}

void nmSPI::begin()
{
    am_hal_iom_initialize(mModule, &mIomHandle);
}

void nmSPI::end()
{
    am_hal_iom_uninitialize(mIomHandle);
}

void nmSPI::beginTransaction(SPISettings settings)
{
    mConfig.eInterfaceMode = AM_HAL_IOM_SPI_MODE;
    mConfig.ui32ClockFreq = settings.getClockFreq();
    switch (settings.getDataMode())
    {
    case SPI_MODE0:
        mConfig.eSpiMode = AM_HAL_IOM_SPI_MODE_0;
        break;
    case SPI_MODE1:
        mConfig.eSpiMode = AM_HAL_IOM_SPI_MODE_1;
        break;
    case SPI_MODE2:
        mConfig.eSpiMode = AM_HAL_IOM_SPI_MODE_2;
        break;
    case SPI_MODE3:
        mConfig.eSpiMode = AM_HAL_IOM_SPI_MODE_3;
        break;
    default:
        mConfig.eSpiMode = AM_HAL_IOM_SPI_MODE_0;
        break;
    }

    am_hal_iom_power_ctrl(mIomHandle, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_iom_configure(mIomHandle, &mConfig);
    am_hal_iom_enable(mIomHandle);

    am_hal_gpio_pinconfig(mPinMap->mosi_pin, mPinMap->mosi_pincfg);
    am_hal_gpio_pinconfig(mPinMap->miso_pin, mPinMap->miso_pincfg);
    am_hal_gpio_pinconfig(mPinMap->sck_pin, mPinMap->sck_pincfg);
//    am_hal_gpio_pinconfig(mPinMap->nce_pin, mPinMap->nce_pincfg);
}

void nmSPI::endTransaction(void)
{
    am_hal_gpio_pinconfig(mPinMap->mosi_pin, g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(mPinMap->miso_pin, g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(mPinMap->sck_pin, g_AM_HAL_GPIO_DISABLE);
//    am_hal_gpio_pinconfig(mPinMap->nce_pin, g_AM_HAL_GPIO_DISABLE);

    am_hal_iom_disable(mIomHandle);
    am_hal_iom_power_ctrl(mIomHandle, AM_HAL_SYSCTRL_DEEPSLEEP, true);
}

uint8_t nmSPI::transfer(uint8_t data)
{
    am_hal_iom_transfer_t transfer;
    uint32_t tx = data;
    uint32_t rx = 0;

    transfer.ui32InstrLen = 0;
    transfer.ui32Instr    = 0;
    transfer.eDirection   = AM_HAL_IOM_FULLDUPLEX;
    transfer.ui32NumBytes = 1;
    transfer.pui32TxBuffer = &tx;
    transfer.pui32RxBuffer = &rx;
    transfer.bContinue      = false;
    transfer.ui8RepeatCount = 0;
    transfer.ui32PauseCondition = 0;
    transfer.ui32StatusSetClr   = 0;
    transfer.uPeerInfo.ui32SpiChipSelect = mPinMap->nce_channel;

    am_hal_iom_spi_blocking_fullduplex(mIomHandle, &transfer);
    return rx;
}

uint16_t nmSPI::transfer16(uint16_t data)
{
    am_hal_iom_transfer_t transfer;
    uint32_t tx = data;
    uint32_t rx = 0;

    transfer.ui32InstrLen = 0;
    transfer.ui32Instr    = 0;
    transfer.eDirection   = AM_HAL_IOM_FULLDUPLEX;
    transfer.ui32NumBytes = 2;
    transfer.pui32TxBuffer = &tx;
    transfer.pui32RxBuffer = &rx;
    transfer.bContinue      = false;
    transfer.ui8RepeatCount = 0;
    transfer.ui32PauseCondition = 0;
    transfer.ui32StatusSetClr   = 0;
    transfer.uPeerInfo.ui32SpiChipSelect = mPinMap->nce_channel;

    am_hal_iom_spi_blocking_fullduplex(mIomHandle, &transfer);
    return rx;
}

void nmSPI::transfer(void *buf, size_t count)
{
    am_hal_iom_transfer_t transfer;

    transfer.ui32InstrLen = 0;
    transfer.ui32Instr    = 0;
    transfer.eDirection   = AM_HAL_IOM_FULLDUPLEX;
    transfer.ui32NumBytes = count;
    transfer.pui32TxBuffer = (uint32_t *)buf;
    transfer.pui32RxBuffer = (uint32_t *)buf;
    transfer.bContinue      = false;
    transfer.ui8RepeatCount = 0;
    transfer.ui32PauseCondition = 0;
    transfer.ui32StatusSetClr   = 0;
    transfer.uPeerInfo.ui32SpiChipSelect = mPinMap->nce_channel;

    am_hal_iom_spi_blocking_fullduplex(mIomHandle, &transfer);
}

void nmSPI::usingInterrupt(int interruptNumber)
{
}

void nmSPI::notUsingInterrupt(int interruptNumber)
{
}

void nmSPI::attachInterrupt()
{
}

void nmSPI::detachInterrupt()
{
}