#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {
    float kp, ki, kd;
    float integral;
    float limit;
    float prev_measured;
    bool  first_run;
} PID;

void  PID_Init(PID *pid, float kp, float ki, float kd, float limit);
float PID_Compute(PID *pid, float setpoint, float measured, float dt);
void  PID_Reset(PID *pid);
void  PID_SetGains(PID *pid, float kp, float ki, float kd);

#ifdef __cplusplus
}
#endif

#endif