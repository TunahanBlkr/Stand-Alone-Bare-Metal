#ifndef TEENSY_PWM_H
#define TEENSY_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "Teensy_Pins.h"
#include "FreeRTOS.h"
#include "task.h"

void analogWrite(uint8_t pin, uint16_t val);
void flexpwm_init(PWM_Type* pwm);
void pwm_init(void);

typedef struct {
	uint8_t type;    // 0=no pwm, 1=flexpwm, 2=quad
	uint8_t module;  // 0-3, 0-3
	uint8_t channel; // 0=X, 1=A, 2=B
	uint8_t muxval;
}pwm_info;

extern pwm_info pwm_pin[55];

#ifdef __cplusplus
}
#endif

#endif