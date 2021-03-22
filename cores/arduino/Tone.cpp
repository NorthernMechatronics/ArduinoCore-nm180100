#ifdef __cplusplus
extern "C" {
#endif

#include <am_mcu_apollo.h>

#ifdef __cplusplus
}
#endif

#include "Arduino.h"

static int counter = 0;

void toneFunction()
{
    counter++;
    if (counter >= 1000000)
    {
        digitalToggle(LED0);
        counter = 0;
    }

}

void toneInit()
{
    /*
    am_hal_ctimer_int_enable(AM_HAL_CTIMER_INT_TIMERA0C0);
    am_hal_ctimer_int_register(AM_HAL_CTIMER_INT_TIMERA0C0, toneFunction);
    NVIC_EnableIRQ(CTIMER_IRQn);
    */
}

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{

}

void noTone(uint8_t _pin)
{

}
