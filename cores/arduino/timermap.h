#ifndef TIMERMAP_H
#define TIMERMAP_H


#include <stdbool.h>
#include <stdint.h>

#include "Common.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

void timermap_ct_init();
int16_t timermap_ct_find(PinName pin);
void timermap_ct_assign(uint8_t seg, uint8_t num, int16_t user);
void timermap_ct_available(PinName pin, uint32_t *seg, uint32_t *num, uint32_t *reg);

#ifdef __cplusplus
}
#endif

#endif /* TIMERMAP_H */
