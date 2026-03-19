#ifndef TEENSY_MOTOR_CONTROLLER_H
#define TEENSY_MOTOR_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "Teensy_PWM.h"
#include "PID_Controller.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "Teensy_Digital.h"
#include <math.h>

#define LUT_SIZE 1024
#define MOTOR_COUNT 2
#define ONE_FULL_CIRCLE_ANGLE 360
#define ONE_DIVIDED_BY_SQRT_3 0.57735026919f // 1/sqrt(3)
#define SQRT_3_DIVIDED_BY_TWO 0.86602540378f // sqrt(3)/2

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define PWM_COUNT_PER_FOC_CYCLE 2
#define FOC_COUNT_PER_SPEED_PID_CYCLE 10
#define PWM_FREQUENCY 16000
#define PWM_PERIOD (1.0f / (float)PWM_FREQUENCY)
#define DT_FOC ((float)(PWM_COUNT_PER_FOC_CYCLE * PWM_PERIOD))
#define DT_SPEED_PID ((float)(DT_FOC * FOC_COUNT_PER_SPEED_PID_CYCLE))

extern float sin_lookup[LUT_SIZE];
extern float cos_lookup[LUT_SIZE];

typedef struct {
    uint16_t MOTOR_1;
    uint16_t MOTOR_2;
    uint16_t MOTOR_3;
}MotorPinMap;

typedef struct {
    float Ia;
    float Ib;
    float RotarAngle;
    float Target_Speed;
    float Current_Speed;
    float Iq_ref;
}FOCInputs;

typedef struct{
    float Iq;
    float Id;
    float Vq;
    float Vd;
    float Va;
    float Vb;
    float Vc;
}FOCOutputs;

typedef struct {
    volatile FOCInputs Motor_FOC[MOTOR_COUNT];
    volatile float Vbus;
    volatile uint8_t LOAD_OK;
    volatile uint8_t PMW_Period_Counter;
    volatile uint8_t FOC_Period_Counter;
}MotorDatas;

void MotorHandler(void *pvParameters);
void Motor1_DataRead(void* pvParameters);
void Motor2_DataRead(void* pvParameters);

extern TaskHandle_t Motor1_DataRead_TaskToken;
extern TaskHandle_t Motor2_DataRead_TaskToken;

#ifdef __cplusplus
}
#endif

#endif