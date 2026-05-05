#include "PID.h"
#include "math.h"

#define DT_MIN 1.0e-6f
#define DT_MAX 1.0f

static float clampf(float val, float lo, float hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

void PID_Init(PID *pid, float kp, float ki, float kd, float limit) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->limit = limit;
    pid->prev_measured = 0.0f;
    pid->integral = 0.0f;
    pid->first_run = true;
}

float PID_Compute(PID *pid, float setpoint, float measured, float dt) {
    if (dt < DT_MIN || dt > DT_MAX || !isfinite(dt))
        dt = 0.01f;

    if (!isfinite(setpoint)) setpoint = 0.0f;
    if (!isfinite(measured)) measured = 0.0f;

    float error = setpoint - measured;

    float p = pid->kp * error;

    pid->integral += error * dt;
    pid->integral = clampf(pid->integral, -pid->limit, pid->limit);
    float i = pid->ki * pid->integral;

    float d = 0.0f;
    if (!pid->first_run) {
        float d_measured = (measured - pid->prev_measured) / dt;
        d = -pid->kd * d_measured;
    }
    pid->first_run = false;

    pid->prev_measured = measured;

    float output = p + i + d;
    float clamped = clampf(output, -pid->limit, pid->limit);

    if (output != clamped) {
        pid->integral -= error * dt;
    }

    return clamped;
}

void PID_Reset(PID *pid) {
    pid->prev_measured = 0.0f;
    pid->integral = 0.0f;
    pid->first_run = true;
}

void PID_SetGains(PID *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}