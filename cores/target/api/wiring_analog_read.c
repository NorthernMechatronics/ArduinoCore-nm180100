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
#include <am_mcu_apollo.h>
#include <am_util.h>

#include <FreeRTOS.h>
#include <task.h>

#include "ArduinoAPI.h"

#define ADC_MAX_CHANNEL (AM_HAL_ADC_SLOT_CHSEL_SE9 + 1)

typedef struct adc_slot_control_s
{
    TaskHandle_t task_handle;
    uint32_t channel;
    uint32_t sample;
} adc_slot_control_t;

typedef struct adc_channel_pincfg_s
{
    uint32_t pin;
    am_hal_gpio_pincfg_t pincfg;
} adc_channel_pincfg_t;

static void *adc_handle;
static am_hal_adc_refsel_e adc_reference = AM_HAL_ADC_REFSEL_INT_2P0;
static am_hal_adc_slot_prec_e adc_precision_mode = AM_HAL_ADC_SLOT_10BIT;
uint32_t adc_hardware_precision = 10;
uint32_t adc_reporting_precision = 10;
static adc_slot_control_t adc_slot_control;
static adc_channel_pincfg_t adc_channel_pinmap[ADC_MAX_CHANNEL] =
{
    {16, { .uFuncSel = AM_HAL_PIN_16_ADCSE0, }},
    {29, { .uFuncSel = AM_HAL_PIN_29_ADCSE1, }},
    {11, { .uFuncSel = AM_HAL_PIN_11_ADCSE2, }},
    {31, { .uFuncSel = AM_HAL_PIN_31_ADCSE3, }},
    {32, { .uFuncSel = AM_HAL_PIN_32_ADCSE4, }},
    {33, { .uFuncSel = AM_HAL_PIN_33_ADCSE5, }},
    {34, { .uFuncSel = AM_HAL_PIN_34_ADCSE6, }},
    {35, { .uFuncSel = AM_HAL_PIN_35_ADCSE7, }},
    {13, { .uFuncSel = AM_HAL_PIN_13_ADCD0PSE8, }},
    {12, { .uFuncSel = AM_HAL_PIN_12_ADCD0NSE9, }},
};

static int32_t adc_get_channel(uint32_t pin)
{
    for (int i = 0; i < ADC_MAX_CHANNEL; i++)
    {
        if (adc_channel_pinmap[i].pin == pin)
        {
            return i;
        }
    }

    return -1;
}

static uint32_t adc_init(void)
{
    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_initialize(0, &adc_handle))
    {
        goto fail;
    }

    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_power_control(adc_handle, AM_HAL_SYSCTRL_WAKE, false))
    {
        goto fail;
    }

    am_hal_adc_config_t adc_config = {.eClock = AM_HAL_ADC_CLKSEL_HFRC,
                                      .ePolarity = AM_HAL_ADC_TRIGPOL_RISING,
                                      .eTrigger = AM_HAL_ADC_TRIGSEL_SOFTWARE,
                                      .eReference = adc_reference,
                                      .eClockMode = AM_HAL_ADC_CLKMODE_LOW_POWER,
                                      .ePowerMode = AM_HAL_ADC_LPMODE0,
                                      .eRepeat = AM_HAL_ADC_REPEATING_SCAN};
    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_configure(adc_handle, &adc_config))
    {
        goto fail;
    }

    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_interrupt_enable(adc_handle, AM_HAL_ADC_INT_CNVCMP))
    {
        goto fail;
    }

    return AM_HAL_STATUS_SUCCESS;

fail:
    return AM_HAL_STATUS_FAIL;
}

static uint32_t adc_deinit(void)
{
    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_interrupt_disable(adc_handle, AM_HAL_ADC_INT_CNVCMP))
    {
        goto fail;
    }

    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_disable(adc_handle))
    {
        goto fail;
    }

    if (AM_HAL_STATUS_SUCCESS != am_hal_pwrctrl_periph_disable(AM_HAL_PWRCTRL_PERIPH_ADC))
    {
        goto fail;
    }

    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_deinitialize(adc_handle))
    {
        goto fail;
    }

    return AM_HAL_STATUS_SUCCESS;

fail:
    return AM_HAL_STATUS_FAIL;
}

static void adc_scan_start(uint32_t channel, uint32_t slot)
{
    am_hal_adc_slot_config_t slot_cfg = {
        .bEnabled = true,
        .bWindowCompare = false,
        .eChannel = channel,
        .eMeasToAvg = AM_HAL_ADC_SLOT_AVG_1,
        .ePrecisionMode = adc_precision_mode,
    };

    adc_init();
    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_configure_slot(adc_handle, slot, &slot_cfg))
    {
        adc_deinit();
        return;
    }

    if (AM_HAL_STATUS_SUCCESS != am_hal_adc_enable(adc_handle))
    {
        adc_deinit();
        return;
    }

    am_hal_adc_sw_trigger(adc_handle);
}

static void adc_scan_complete()
{
    uint32_t samples = 1;
    am_hal_adc_sample_t adc_sample;

    am_hal_adc_samples_read(adc_handle, true, NULL, &samples, &adc_sample);
    adc_slot_control.sample = AM_HAL_ADC_FIFO_SAMPLE(adc_sample.ui32Sample);
}

int analogRead(pin_size_t pinNumber)
{
    int32_t channel = adc_get_channel(pinNumber);

    if (channel == -1)
    {
        return 0;
    }

    am_hal_gpio_pinconfig(pinNumber, adc_channel_pinmap[channel].pincfg);
    adc_slot_control.task_handle = xTaskGetCurrentTaskHandle();
    adc_slot_control.sample = 0;
    adc_slot_control.channel = channel;
    adc_scan_start(channel, 0);

    xTaskNotifyWait(0, 1, NULL, portMAX_DELAY);

    adc_scan_complete();
    adc_deinit();
    am_hal_gpio_pinconfig(pinNumber, g_AM_HAL_GPIO_DISABLE);

    uint32_t sample = adc_slot_control.sample;
    int32_t shift = adc_hardware_precision - adc_reporting_precision;
    sample = (sample >> shift);

    memset(&adc_slot_control, 0, sizeof(adc_slot_control_t));

    return sample;
}

void analogReference(uint8_t mode)
{
    switch (mode)
    {
    case AM_HAL_ADC_REFSEL_INT_1P5:
        adc_reference = mode;
        break;

    case AM_HAL_ADC_REFSEL_INT_2P0:
        adc_reference = mode;
        break;

    default:
        adc_reference = AM_HAL_ADC_REFSEL_INT_2P0;
        break;
    }
}

void analogReadResolution(uint32_t resolution)
{
    switch (resolution)
    {
    case 8:
        adc_precision_mode = AM_HAL_ADC_SLOT_8BIT;
        adc_hardware_precision = 8;
        adc_reporting_precision = 8;
        break;

    case 10:
        adc_precision_mode = AM_HAL_ADC_SLOT_10BIT;
        adc_hardware_precision = 10;
        adc_reporting_precision = 10;
        break;

    case 12:
        adc_precision_mode = AM_HAL_ADC_SLOT_12BIT;
        adc_hardware_precision = 12;
        adc_reporting_precision = 12;
        break;

    case 14:
        adc_precision_mode = AM_HAL_ADC_SLOT_14BIT;
        adc_hardware_precision = 14;
        adc_reporting_precision = 14;
        break;

    default:
        adc_precision_mode = AM_HAL_ADC_SLOT_14BIT;
        adc_hardware_precision = 14;
        adc_reporting_precision = resolution;
        break;
    }
}

void analogWrite(pin_size_t pinNumber, int value)
{
}

void analogWriteResolution(uint32_t resolution)
{
}

void am_adc_isr(void)
{
    BaseType_t bContextSwitch = pdFALSE;
    uint32_t adc_interrupt_status;

    am_hal_adc_interrupt_status(adc_handle, &adc_interrupt_status, true);
    am_hal_adc_interrupt_clear(adc_handle, adc_interrupt_status);

    if (adc_interrupt_status & AM_HAL_ADC_INT_CNVCMP)
    {
        xTaskNotifyFromISR(adc_slot_control.task_handle, 0, eNoAction, &bContextSwitch);
        portYIELD_FROM_ISR(bContextSwitch);
    }
}