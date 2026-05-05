#include "Motor_Controller.h"

MotorDatas Motor_Control_Input = {0};

TaskHandle_t MotorHandlerTask = NULL;

static bool BitMapControl(MotorPinMap* Motor_Pin_Map, uint16_t motor_count) {
    uint32_t LowPinControl  = 0;
    uint32_t HighPinControl = 0;
    MotorPinMap* pinMap;

    for(int i = 0; i < motor_count; i++) {
        pinMap = Motor_Pin_Map + i;

        //-----MOTOR_1-----
        if(pinMap ->MOTOR_1 > 37) return false;
        if(pinMap ->MOTOR_1 < 32) {
            if(LowPinControl & (1 << pinMap ->MOTOR_1)) return false;
            LowPinControl |= 1 << pinMap ->MOTOR_1;
        } else {
            if(HighPinControl & (1 << (pinMap ->MOTOR_1 - 32))) return false;
            HighPinControl |= 1 << (pinMap ->MOTOR_1 - 32);
        }

        //-----MOTOR_2-----
        if(pinMap ->MOTOR_2 > 37) return false;
        if(pinMap ->MOTOR_2 < 32) {
            if(LowPinControl & (1 << pinMap ->MOTOR_2)) return false;
            LowPinControl |= 1 << pinMap ->MOTOR_2;
        } else {
            if(HighPinControl & (1 << (pinMap ->MOTOR_2 - 32))) return false;
            HighPinControl |= 1 << (pinMap ->MOTOR_2 - 32);
        }

        //-----MOTOR_3-----
        if(pinMap ->MOTOR_3 > 37) return false;
        if(pinMap ->MOTOR_3 < 32) {
            if(LowPinControl & (1 << pinMap ->MOTOR_3)) return false;
            LowPinControl |= 1 << pinMap ->MOTOR_3;
        } else {
            if(HighPinControl & (1 << (pinMap ->MOTOR_3 - 32))) return false;
            HighPinControl |= 1 << (pinMap ->MOTOR_3 - 32);
        }
    }

    return true;
}

static uint16_t CreateBitMap(MotorPinMap* Motor_Pin_Map, uint16_t motor_count) {

    if(!BitMapControl(Motor_Pin_Map, motor_count)) return 0;

    uint16_t bitMap = 0;
    MotorPinMap* pinMap;
    pwm_info* FlexPwm;

    for(int i = 0; i < motor_count; i++) {
        pinMap = Motor_Pin_Map + i;

        FlexPwm = pwm_pin + pinMap->MOTOR_1;
        if(FlexPwm->type != 1) return 0;
        bitMap |= 1 << FlexPwm->module;

        FlexPwm = pwm_pin + pinMap->MOTOR_2;
        if(FlexPwm->type != 1) return 0;
        bitMap |= 1 << FlexPwm->module;

        FlexPwm = pwm_pin + pinMap->MOTOR_3;
        if(FlexPwm->type != 1) return 0;
        bitMap |= 1 << FlexPwm->module;
    }

    return bitMap;
}

static void ConfigurePWMforMotorRun(uint16_t MotorPin) {
    pwm_info* p = pwm_pin + MotorPin;
    Pins* port = Teensy_Pins + MotorPin;

    PWM_Type* Flexpwm;

    switch ((p->module >> 2) & 3) {
		case 0:  Flexpwm = PWM1; break;
		case 1:  Flexpwm = PWM2; break;
		case 2:  Flexpwm = PWM3; break;
		default: Flexpwm = PWM4;
	}

    switch (p->channel) {
	case 0: // X
		Flexpwm->OUTEN |= PWM_OUTEN_PWMX_EN(1 << (p->module & 3));
		break;
	case 1: // A
		Flexpwm->OUTEN |= PWM_OUTEN_PWMA_EN(1 << (p->module & 3));
		break;
	case 2: // B
		Flexpwm->OUTEN |= PWM_OUTEN_PWMB_EN(1 << (p->module & 3));
	}

    IOMUXC->SW_MUX_CTL_PAD[port->mux] = ( IOMUXC->SW_MUX_CTL_PAD[port->mux] & ~(port->mux_mask) ) | p->muxval;
}

static uint16_t MotorInit(MotorPinMap* Motor_Pin_Map, uint16_t motor_count) {
    uint16_t bitMap = CreateBitMap(Motor_Pin_Map, motor_count);

    if(!bitMap) {
        return;
    }

    PWM1->MCTRL &= ~PWM_MCTRL_RUN((bitMap >> 0) & 0xF);
    PWM2->MCTRL &= ~PWM_MCTRL_RUN((bitMap >> 4) & 0xF);
    PWM3->MCTRL &= ~PWM_MCTRL_RUN((bitMap >> 8) & 0xF);
    PWM4->MCTRL &= ~PWM_MCTRL_RUN((bitMap >> 12) & 0xF);

    PWM3->MCTRL &= ~PWM_MCTRL_RUN(1 << 2);

    PWM1->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 0) & 0xF);
    PWM2->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 4) & 0xF);
    PWM3->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 8) & 0xF);
    PWM4->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 12) & 0xF);

    PWM3->MCTRL |= PWM_MCTRL_CLDOK(1 << 2);

    MotorPinMap* pinMap;

    for(int i = 0; i < motor_count; i++) {
        pinMap = Motor_Pin_Map + i;
        
        ConfigurePWMforMotorRun(pinMap->MOTOR_1);
        ConfigurePWMforMotorRun(pinMap->MOTOR_2);
        ConfigurePWMforMotorRun(pinMap->MOTOR_3);
    }

    PWM3->SM[2].VAL0 = 0;

    PWM3->SM[2].STS = PWM_STS_CMPF(1 << 2);
    PWM3->SM[2].INTEN |= PWM_INTEN_CMPIE(1 << 0); //VAL0 Compare Interrupt Enable

    taskENTER_CRITICAL();
    PWM1->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 0) & 0xF);
    PWM2->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 4) & 0xF);
    PWM3->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 8) & 0xF);
    PWM4->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 12) & 0xF);

    PWM3->MCTRL |= PWM_MCTRL_LDOK(1 << 2);

    PWM1->MCTRL |= PWM_MCTRL_RUN((bitMap >> 0) & 0xF);
    PWM2->MCTRL |= PWM_MCTRL_RUN((bitMap >> 4) & 0xF);
    PWM3->MCTRL |= PWM_MCTRL_RUN((bitMap >> 8) & 0xF);
    PWM4->MCTRL |= PWM_MCTRL_RUN((bitMap >> 12) & 0xF);

    PWM3->MCTRL |= PWM_MCTRL_RUN(1 << 2);
    taskEXIT_CRITICAL();

    __DSB();
    __ISB();

    NVIC_SetPriority(PWM3_2_IRQn, 3);
    NVIC_EnableIRQ(PWM3_2_IRQn);

    return bitMap;
}

void MotorHandler(void *pvParameters) {

    (void)pvParameters;
    FOCInputs *Inputs = Motor_Control_Input.Motor_FOC;
    FOCOutputs Outputs[MOTOR_COUNT] = {0};

    MotorPinMap Motor_Pin_Map[MOTOR_COUNT] = {{3, 4, 5}, {6, 7, 8}};

    CreateLUT();

    PID SpeedPID_1 = {0};
    PID Iq_PID_1   = {0};
    PID Id_PID_1   = {0};
    
    PID SpeedPID_2 = {0};
    PID Iq_PID_2   = {0};
    PID Id_PID_2   = {0};

    PID_Init(&SpeedPID_1, 1.0f, 0.5f, 0.0f, 30.0f);
    PID_Init(&Iq_PID_1, 1.0f, 0.5f, 0.0f, 24.0f);
    PID_Init(&Id_PID_1, 1.0f, 0.5f, 0.0f, 24.0f);

    PID_Init(&SpeedPID_2, 1.0f, 0.5f, 0.0f, 30.0f);
    PID_Init(&Iq_PID_2, 1.0f, 0.5f, 0.0f, 24.0f);
    PID_Init(&Id_PID_2, 1.0f, 0.5f, 0.0f, 24.0f);

    //SPI ile ESC baslatma

    uint16_t bitMap = MotorInit(Motor_Pin_Map, MOTOR_COUNT);

    while(1) {

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        taskENTER_CRITICAL();

        // Read Inputs that comes from I2C_1
        // Read Inputs that comes from I2C_2

        PWM1->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 0) & 0xF);
        PWM2->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 4) & 0xF);
        PWM3->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 8) & 0xF);
        PWM4->MCTRL |= PWM_MCTRL_CLDOK((bitMap >> 12) & 0xF);

        Motor_Control_Input.FOC_Period_Counter++;

        ClarkPark((Inputs + 0), (Outputs + 0));
        ClarkPark((Inputs + 1), (Outputs + 1));

        PIDCalculation(&Motor_Control_Input, (Inputs + 0), (Outputs + 0), &SpeedPID_1, &Iq_PID_1, &Id_PID_1);
        PIDCalculation(&Motor_Control_Input, (Inputs + 1), (Outputs + 1), &SpeedPID_2, &Iq_PID_2, &Id_PID_2);

        InverseClarkPark((Inputs + 0), (Outputs + 0));
        InverseClarkPark((Inputs + 1), (Outputs + 1));

        SetPWM(Motor_Pin_Map[0], (Outputs + 0), Motor_Control_Input.Vbus);
        SetPWM(Motor_Pin_Map[1], (Outputs + 1), Motor_Control_Input.Vbus);

        PWM1->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 0) & 0xF);
        PWM2->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 4) & 0xF);
        PWM3->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 8) & 0xF);
        PWM4->MCTRL |= PWM_MCTRL_LDOK((bitMap >> 12) & 0xF);

        taskEXIT_CRITICAL();
    }
}

void PWM3_2_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(MotorHandlerTask != NULL) {
        vTaskNotifyGiveFromISR(MotorHandlerTask, &xHigherPriorityTaskWoken);
    }
    //I2C_1 Active
    //I2C_2 Active

    PWM3->SM[2].STS = PWM_STS_CMPF_MASK;

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}