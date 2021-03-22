#ifndef WIRING_ANALOG_H
#define WIRING_ANALOG_H

#ifdef __cplusplus
extern "C" {
#endif

void pwmWrite(pin_size_t pinNumber, uint32_t seg, uint32_t num, uint32_t reg, uint16_t period, uint16_t value, uint32_t clock);

#ifdef __cplusplus
}
#endif

#endif /* WIRING_ANALOG_H */
