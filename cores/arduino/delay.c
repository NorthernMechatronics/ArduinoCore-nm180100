#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "am_util_delay.h"

unsigned long millis(void)
{
  return 0;
}

unsigned long micros( void )
{
  return 0;
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
