#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "am_mcu_apollo.h"
#include "am_util_delay.h"
#include "system_config.h"

unsigned long millis(void)
{
  return am_hal_ctimer_read(
      SYSTEM_TIMER_COUNTER_MS_NUM,
      SYSTEM_TIMER_COUNTER_MS_SEG);
}

unsigned long micros( void )
{
  return am_hal_stimer_counter_get();
}

void delay( unsigned long ms )
{
  if (ms == 0)
  {
    return;
  }

  am_util_delay_ms(ms);
}

void delayMicroseconds( unsigned int us )
{
  if (us == 0)
  {
    return;
  }

  am_util_delay_us(us);
}


#ifdef __cplusplus
}
#endif
