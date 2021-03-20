#ifndef PINMAP_H
#define PINMAP_H

#include <stdbool.h>
#include <stdint.h>

#include "Common.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName pin;
    uint32_t peripheral;
    uint32_t function;
} PinMap;

typedef struct {
    uint32_t count;
    const PinName* pins;
} PinList;

typedef struct {
    uint32_t count;
    const int* peripheral;
} PeripheralList;

uint32_t pinmap_peripheral(PinName pin, const PinMap* map);
uint32_t pinmap_function(PinName pin, const PinMap* map);
uint32_t pinmap_find_peripheral(PinName pin, const PinMap* map);
uint32_t pinmap_find_function(PinName pin, const PinMap* map);
bool pinmap_find_peripheral_pins(const PinList* whitelist, const PinList* blacklist, int per, const PinMap* const* maps, PinName** pins, uint32_t count);
bool pinmap_list_has_pin(const PinList* list, PinName pin);
bool pinmap_list_has_peripheral(const PeripheralList* list, int peripheral);
const PinList* pinmap_restricted_pins(void);
const PeripheralList* pinmap_uart_restricted_peripherals(void);
const PinList* pinmap_gpio_restricted_pins(void);

const PinList* pinmap_ff_default_pins(void);
const char* pinmap_ff_default_pin_to_string(PinName pin);

#ifdef __cplusplus
}
#endif

#endif
