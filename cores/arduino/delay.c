#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

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

  uint32_t start = micros();

  while (ms > 0)
  {
    while (ms > 0 && (micros() - start) >= 1000)
    {
      ms--;
      start += 1000;
    }
  }
}


#ifdef __cplusplus
}
#endif
