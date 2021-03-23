#ifdef __cplusplus
extern "C" {
#endif

#include <am_mcu_apollo.h>

#ifdef __cplusplus
}
#endif

#include "Common.h"
#include "pinmap.h"
#include "timermap.h"
#include "wiring_analog.h"
#include "Arduino.h"

static uint32_t active_num;
static uint32_t active_seg;
static uint32_t active_pin;

extern "C" {

void am_stimer_cmpr0_isr(void)
{
    am_hal_stimer_int_clear(AM_HAL_STIMER_INT_COMPAREA);

    am_hal_gpio_pinconfig(active_pin, g_AM_HAL_GPIO_DISABLE);
    timermap_ct_assign(active_seg, active_num, -1);
    am_hal_ctimer_stop(active_num, active_seg ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA);

    am_hal_stimer_int_disable(AM_HAL_STIMER_INT_COMPAREA);
    NVIC_DisableIRQ(STIMER_CMPR0_IRQn);
}

}

void tone(pin_size_t pin, unsigned int frequency, unsigned long duration)
{
    uint32_t seg;
    uint32_t num;
    uint32_t reg;

    timermap_ct_available((PinName)pin, &seg, &num, &reg);

    if (seg == -1)
    {
        return;
    }

    uint32_t period, cycle;
    period = 3e6 / frequency;
    cycle = period >> 1;

    pwmWrite((PinName)pin,
             seg ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA,
             num, reg, period, cycle, AM_HAL_CTIMER_HFRC_3MHZ);

    active_num = num;
    active_seg = seg;
    active_pin = pin;

    if (duration)
    {
        am_hal_stimer_compare_delta_set(0, duration * 1000);
        am_hal_stimer_int_enable(AM_HAL_STIMER_INT_COMPAREA);
        NVIC_EnableIRQ(STIMER_CMPR0_IRQn);
    }
}

void noTone(pin_size_t pin)
{
    uint32_t seg;
    uint32_t num;
    uint32_t reg;

    timermap_ct_available((PinName)pin, &seg, &num, &reg);

    if (seg == -1)
    {
        return;
    }

    timermap_ct_assign(seg, num, -1);
    am_hal_ctimer_stop(num, seg ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA);
}
