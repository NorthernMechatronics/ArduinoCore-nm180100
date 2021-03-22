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
    period = 1e12 / frequency;
    cycle = period >> 1;

    pwmWrite((PinName)pin,
             seg ? AM_HAL_CTIMER_TIMERB : AM_HAL_CTIMER_TIMERA,
             num, reg, period, cycle, AM_HAL_CTIMER_HFRC_12MHZ);
}

void noTone(pin_size_t pin)
{

}
