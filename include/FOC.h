#ifndef FOC_H
#define FOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PID.h"
#include "PWM.h"
#include "stdint.h"
#include "math.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define MOTOR_COUNT 2
#define ONE_FULL_CIRCLE_ANGLE 360
#define ONE_DIVIDED_BY_SQRT_3 0.57735026919f // 1/sqrt(3)
#define SQRT_3_DIVIDED_BY_TWO 0.86602540378f // sqrt(3)/2

#define FOC_COUNT_PER_SPEED_PID_CYCLE 10
#define FREQUENCY PWM_FREQUENCY
#define PWM_PERIOD (1.0f / (float)FREQUENCY)
#define DT_FOC ((float) PWM_PERIOD)
#define DT_SPEED_PID ((float)(DT_FOC * FOC_COUNT_PER_SPEED_PID_CYCLE))

#define LUT_SIZE 1024

extern float sin_lookup[LUT_SIZE];
extern float cos_lookup[LUT_SIZE];

typedef struct {
    uint16_t MOTOR_1;
    uint16_t MOTOR_2;
    uint16_t MOTOR_3;
}MotorPinMap;

typedef struct {
    float Ia;
    float Ib_previous;
    float Ib_current;
    float Ib_corrected;
    float RotarAngle_previous;
    float RotarAngle_later;
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
    FOCInputs Motor_FOC[MOTOR_COUNT];
    float Vbus;
    uint8_t FOC_Period_Counter;
}MotorDatas;

void ClarkPark(FOCInputs* Inputs, FOCOutputs* Outputs);
void PIDCalculation(MotorDatas* Motor_Control, FOCInputs* Inputs, FOCOutputs* Outputs, PID* SpeedPID, PID* Iq_PID, PID* Id_PID);
void InverseClarkPark(FOCInputs* Inputs, FOCOutputs* Outputs);
void SetPWM(MotorPinMap Pins, FOCOutputs* Outputs, float Vbus);
void ApplyDutyCycle(uint16_t pin, float DutyCycle);
void CreateLUT();

#ifdef __cplusplus
}
#endif

#endif