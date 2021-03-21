#ifndef VARIANT_H
#define VARIANT_H

#include "PeripheralNames.h"
#include "PeripheralPins.h"

#ifdef __cplusplus
#include "Peripherals.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define INTERNAL1V5 AM_HAL_ADC_REFSEL_INT_1P5
#define INTERNAL2V0 AM_HAL_ADC_REFSEL_INT_2P0
#define DEFAULT     AM_HAL_ADC_REFSEL_INT_2P0

void initVariant();

#ifdef __cplusplus
}
#endif


#endif /* VARIANT_H */
