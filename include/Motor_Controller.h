#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "FreeRTOS.h"
#include "task.h"
#include "PID.h"
#include "FOC.h"
#include "Digital.h"
#include "stdbool.h"

void MotorHandler(void *pvParameters);
extern TaskHandle_t MotorHandlerTask;

#ifdef __cplusplus
}
#endif

#endif