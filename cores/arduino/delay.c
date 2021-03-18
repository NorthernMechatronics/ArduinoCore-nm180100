#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "am_util_delay.h"

unsigned long millis(void)
{
  am_util_delay_ms(1);
  return 0;
}

unsigned long micros( void )
{
  am_util_delay_ms(1);
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


#ifdef __cplusplus
}
#endif
