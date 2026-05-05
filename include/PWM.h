#ifndef PWM_H
#define PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "Pins.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
	uint8_t type;    // 0=no pwm, 1=flexpwm, 2=quad
	uint8_t module;  // 0-3, 0-3
	uint8_t channel; // 0=X, 1=A, 2=B
	uint8_t muxval;
}pwm_info;

typedef enum {
	LEFT_ALIGNED,
	CENTER,
	RIGHT_ALIGNED
} PWM_Align;

void analogWrite(uint8_t pin, uint16_t val);
void pwm_init(void);
extern pwm_info pwm_pin[55];

#define PWM_CLK_HZ CCM_IPG_CLK_HZ
#define PWM_FREQUENCY 8000
#define PWM_ALIGN_TYPE CENTER

#ifdef __cplusplus
}
#endif

#endif