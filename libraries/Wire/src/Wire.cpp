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
  : mModule(module), mPinMap(pinMap)
{
    am_hal_queue_init(&mTxQueue, mTxBuffer, 1, AM_HAL_IOM_MAX_TXNSIZE_I2C);
    am_hal_queue_init(&mRxQueue, mRxBuffer, 1, AM_HAL_IOM_MAX_TXNSIZE_I2C);
    mFrequency = AM_HAL_IOM_100KHZ;
}

void nmI2C::begin()
{
    mConfig.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    mConfig.ui32ClockFreq = mFrequency;
    am_hal_iom_initialize(mModule, &mIomHandle);
    am_hal_iom_power_ctrl(mIomHandle, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_iom_configure(mIomHandle, &mConfig);
    am_hal_iom_enable(mIomHandle);

    am_hal_gpio_pinconfig(mPinMap->sda_pin, mPinMap->sda_pincfg);
    am_hal_gpio_pinconfig(mPinMap->sck_pin, mPinMap->sck_pincfg);
}

void nmI2C::begin(uint8_t address)
{
    mAddress = address;
}

void nmI2C::end()
{
    am_hal_gpio_pinconfig(mPinMap->sda_pin, g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(mPinMap->sck_pin, g_AM_HAL_GPIO_DISABLE);
    am_hal_iom_disable(mIomHandle);
    am_hal_iom_power_ctrl(mIomHandle, AM_HAL_SYSCTRL_DEEPSLEEP, true);
    am_hal_iom_uninitialize(mIomHandle);
}

void nmI2C::setClock(uint32_t freq)
{
    mFrequency = freq;
}

void nmI2C::beginTransmission(uint8_t address)
{
    mAddress = address;
}

uint8_t nmI2C::endTransmission(bool stopBit)
{
    am_hal_iom_transfer_t transfer;

    uint32_t len = am_hal_queue_items_left(&mTxQueue);
    am_hal_queue_item_get(&mTxQueue, mTransferBuffer, len);

    transfer.ui32InstrLen = 0;
    transfer.ui32Instr    = 0;
    transfer.eDirection   = AM_HAL_IOM_TX;
    transfer.ui32NumBytes = len;
    transfer.pui32TxBuffer = (uint32_t *)mTransferBuffer;
    transfer.pui32RxBuffer = NULL;
    transfer.bContinue      = !stopBit;
    transfer.ui8RepeatCount = 0;
    transfer.ui32PauseCondition = 0;
    transfer.ui32StatusSetClr   = 0;
    transfer.uPeerInfo.ui32I2CDevAddr = mAddress;

    if (AM_HAL_STATUS_SUCCESS == am_hal_iom_blocking_transfer(mIomHandle, &transfer))
    {
        return len;
    }

    return 0;
}

uint8_t nmI2C::endTransmission(void)
{
    return endTransmission(true);
}

size_t nmI2C::requestFrom(uint8_t address, size_t len, bool stopBit)
{
    am_hal_iom_transfer_t transfer;

    transfer.ui32InstrLen = 0;
    transfer.ui32Instr    = 0;
    transfer.eDirection   = AM_HAL_IOM_RX;
    transfer.ui32NumBytes = len;
    transfer.pui32TxBuffer = NULL;
    transfer.pui32RxBuffer = (uint32_t *)mTransferBuffer;
    transfer.bContinue      = !stopBit;
    transfer.ui8RepeatCount = 0;
    transfer.ui32PauseCondition = 0;
    transfer.ui32StatusSetClr   = 0;
    transfer.uPeerInfo.ui32I2CDevAddr = mAddress;

    if (AM_HAL_STATUS_SUCCESS == am_hal_iom_blocking_transfer(mIomHandle, &transfer))
    {
        am_hal_queue_item_add(&mRxQueue, mTransferBuffer, len);
        return len;
    }

    return 0;
}

size_t nmI2C::requestFrom(uint8_t address, size_t len)
{
    return requestFrom(address, len, true);
}

void nmI2C::onReceive(void (*)(int))
{
}

void nmI2C::onRequest(void (*)(void))
{
}

int nmI2C::available()
{
    return am_hal_queue_items_left(&mRxQueue);
}

int nmI2C::read()
{
    if (am_hal_queue_empty(&mRxQueue))
    {
        return -1;
    }

    uint8_t item;
    am_hal_queue_item_get(&mRxQueue, &item, 1);
    return item;
}

int read(uint8_t *buffer, size_t size)
{
    if (am_hal_queue_empty(&mRxQueue))
    {
        return -1;
    }

    return am_hal_queue_item_get(buffer, size);
}

int nmI2C::peek()
{
    if (am_hal_queue_empty(&mRxQueue))
    {
        return -1;
    }

    uint8_t *item = am_hal_queue_peek(&mRxQueue);
    return *item;
}

size_t nmI2C::write(uint8_t n)
{
    return write(&n, 1);
}

size_t nmI2C::write(const uint8_t *buffer, size_t size)
{
    bool bStatus = am_hal_queue_item_add(&mTxQueue, buffer, size);
    return bStatus ? size : 0;
}