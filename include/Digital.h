#ifndef DIGITAL_H
#define DIGITAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "Pins.h"
#include "stdbool.h"

typedef enum {
    INPUT,
    OUTPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    OUTPUT_OPENDRAIN,
    INPUT_DISABLE
} PIN_MODE;

#define LOW   0
#define HIGH  1

void pinMode(uint8_t pin, PIN_MODE mode);
void digitalToggle(uint8_t pin);
void digitalWrite(uint8_t pin, uint8_t val);
bool digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif