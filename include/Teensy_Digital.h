#ifndef TEENSY_DIGITAL_H
#define TEENSY_DIGITAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "Teensy_Pins.h"

#define INPUT             0
#define OUTPUT            1
#define INPUT_PULLUP      2
#define INPUT_PULLDOWN	  3
#define OUTPUT_OPENDRAIN  4
#define INPUT_DISABLE	  5

#define LOW   0
#define HIGH  1

void pinMode(uint8_t pin, uint8_t mode);

void digitalToggle(uint8_t pin);
void digitalWrite(uint8_t pin, uint8_t val);
uint8_t digitalRead(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif