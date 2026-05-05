#include "FOC.h"

float sin_lookup[LUT_SIZE];
float cos_lookup[LUT_SIZE];

void ClarkPark(FOCInputs* Inputs, FOCOutputs* Outputs) {
    float I_alpha = Inputs->Ia;
    float I_beta  = (Inputs->Ia + 2.0f * Inputs->Ib_corrected) * ONE_DIVIDED_BY_SQRT_3;
    uint16_t LUT_INDEX = (uint16_t)(Inputs->RotarAngle_previous * LUT_SIZE / ONE_FULL_CIRCLE_ANGLE) & (LUT_SIZE - 1);

    Outputs->Id = I_beta * sin_lookup[LUT_INDEX] + I_alpha * cos_lookup[LUT_INDEX];
    Outputs->Iq = I_beta * cos_lookup[LUT_INDEX] - I_alpha * sin_lookup[LUT_INDEX];
}

void PIDCalculation(MotorDatas* Motor_Control, FOCInputs* Inputs, FOCOutputs* Outputs, PID* SpeedPID, PID* Iq_PID, PID* Id_PID) {
    if(Motor_Control->FOC_Period_Counter >= FOC_COUNT_PER_SPEED_PID_CYCLE) {
        Inputs->Iq_ref = PID_Compute(SpeedPID, Inputs->Target_Speed, Inputs->Current_Speed, DT_SPEED_PID);
        Motor_Control->FOC_Period_Counter = 0;
    }
    Outputs->Vq = PID_Compute(Iq_PID, Inputs->Iq_ref, Outputs->Iq, DT_FOC);
    Outputs->Vd = PID_Compute(Id_PID, 0.0f, Outputs->Id, DT_FOC);
}

void InverseClarkPark(FOCInputs* Inputs, FOCOutputs* Outputs) {
    uint16_t LUT_INDEX = (uint16_t)(Inputs->RotarAngle_later * LUT_SIZE / ONE_FULL_CIRCLE_ANGLE) & (LUT_SIZE - 1);

    float V_alpha = Outputs->Vd*cos_lookup[LUT_INDEX] - Outputs->Vq*sin_lookup[LUT_INDEX];
    float V_beta  = Outputs->Vd*sin_lookup[LUT_INDEX] + Outputs->Vq*cos_lookup[LUT_INDEX];

    Outputs->Va = V_alpha;
    Outputs->Vb = -0.5f * V_alpha + SQRT_3_DIVIDED_BY_TWO * V_beta;
    Outputs->Vc = -0.5f * V_alpha - SQRT_3_DIVIDED_BY_TWO * V_beta;
}

void SetPWM(MotorPinMap Pins, FOCOutputs* Outputs, float Vbus) {
    float inv_Vbus = 1.0f/Vbus;

    float DutyA = (Outputs->Va + 0.5f*Vbus) * inv_Vbus;
    float DutyB = (Outputs->Vb + 0.5f*Vbus) * inv_Vbus;
    float DutyC = (Outputs->Vc + 0.5f*Vbus) * inv_Vbus;

    ApplyDutyCycle(Pins.MOTOR_1, DutyA);
    ApplyDutyCycle(Pins.MOTOR_2, DutyB);
    ApplyDutyCycle(Pins.MOTOR_3, DutyC);
}

void ApplyDutyCycle(uint16_t pin, float DutyCycle) {
    pwm_info* p = pwm_pin + pin;

    PWM_Type* flexpwm;

	switch ((p->module >> 2) & 3) {
		case 0:  flexpwm = PWM1; break;
		case 1:  flexpwm = PWM2; break;
		case 2:  flexpwm = PWM3; break;
		default: flexpwm = PWM4;
	}

    uint8_t  submodule = p->module & 3;
    uint32_t modulo    = flexpwm->SM[submodule].VAL1;

    switch (p->channel) {
    case 0:
        flexpwm->SM[submodule].VAL0 = modulo*(1 - DutyCycle);
        flexpwm->OUTEN |= PWM_OUTEN_PWMX_EN(1 << (p->module & 3));
        break;
    case 1:
        switch(PWM_ALIGN_TYPE) {
        case LEFT_ALIGNED:
            flexpwm->SM[submodule].VAL2 =  0;
            flexpwm->SM[submodule].VAL3 =  modulo * DutyCycle;
            break;
        case CENTER:
            flexpwm->SM[submodule].VAL2 = -modulo * DutyCycle;
            flexpwm->SM[submodule].VAL3 =  modulo * DutyCycle;
            break;
        case RIGHT_ALIGNED:
            flexpwm->SM[submodule].VAL2 =  modulo*(1 - DutyCycle);
            flexpwm->SM[submodule].VAL3 =  modulo;
        }
        flexpwm->OUTEN |= PWM_OUTEN_PWMA_EN(1 << (p->module & 3));
        break;
    case 2:
        switch(PWM_ALIGN_TYPE) {
        case LEFT_ALIGNED:
            flexpwm->SM[submodule].VAL4 =  0;
            flexpwm->SM[submodule].VAL5 =  modulo * DutyCycle;
            break;
        case CENTER:
            flexpwm->SM[submodule].VAL4 = -modulo * DutyCycle;
            flexpwm->SM[submodule].VAL5 =  modulo * DutyCycle;
            break;
        case RIGHT_ALIGNED:
            flexpwm->SM[submodule].VAL4 =  modulo*(1 - DutyCycle);
            flexpwm->SM[submodule].VAL5 =  modulo;
        }
        flexpwm->OUTEN |= PWM_OUTEN_PWMB_EN(1 << (p->module & 3));
    }
}

void CreateLUT() {
    float angle;
    for (int i = 0; i < LUT_SIZE; i++) {
        angle = 2.0f * M_PI * i / LUT_SIZE;
        sin_lookup[i] = sinf(angle);
        cos_lookup[i] = cosf(angle);
    }
}