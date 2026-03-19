#include "Teensy_Motor_Controller.h"

float sin_lookup[LUT_SIZE];
float cos_lookup[LUT_SIZE];

static void MotorInit(MotorPinMap* Motor_Pin_Map, uint16_t motor_count);
static void CreateLUT();
static void ConfigurePWMforMotorRun(uint16_t MotorPin);
static uint8_t BitMapControl(MotorPinMap* Motor_Pin_Map, uint16_t motor_count);
static uint16_t CreateBitMap(MotorPinMap* Motor_Pin_Map, uint16_t motor_count);

static void ClarkPark(volatile FOCInputs* Inputs, FOCOutputs* Outputs);
static void PIDCalculation(volatile MotorDatas* Motor_Control, volatile FOCInputs* Inputs, FOCOutputs* Outputs, PID* SpeedPID, PID* Iq_PID, PID* Id_PID);
static void InverseClarkPark(volatile FOCInputs* Inputs, FOCOutputs* Outputs);
static void SetPWM(MotorPinMap Pins, FOCOutputs* Outputs, float Vbus);
static void ApplyDutyCycle(uint16_t pin, float DutyCycle);

EventGroupHandle_t xMotorSyncEvents;

TaskHandle_t Motor1_DataRead_TaskToken = NULL;
TaskHandle_t Motor2_DataRead_TaskToken = NULL;

#define M1_READY_BIT (1 << 0)
#define M2_READY_BIT (1 << 1)

volatile MotorDatas Motor_Control_Input = {0};

void MotorHandler(void *pvParameters) {

    (void)pvParameters;
    volatile FOCInputs *Inputs = Motor_Control_Input.Motor_FOC;
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

    MotorInit(Motor_Pin_Map, MOTOR_COUNT);

    while(1) {

        Motor_Control_Input.LOAD_OK = 1;

        xEventGroupWaitBits( xMotorSyncEvents, (M1_READY_BIT | M2_READY_BIT), pdTRUE, pdTRUE, portMAX_DELAY );

        Motor_Control_Input.FOC_Period_Counter++;

        ClarkPark((Inputs + 0), (Outputs + 0));
        ClarkPark((Inputs + 1), (Outputs + 1));

        PIDCalculation(&Motor_Control_Input, (Inputs + 0), (Outputs + 0), &SpeedPID_1, &Iq_PID_1, &Id_PID_1);
        PIDCalculation(&Motor_Control_Input, (Inputs + 1), (Outputs + 1), &SpeedPID_2, &Iq_PID_2, &Id_PID_2);

        InverseClarkPark((Inputs + 0), (Outputs + 0));
        InverseClarkPark((Inputs + 1), (Outputs + 1));

        SetPWM(Motor_Pin_Map[0], (Outputs + 0), Motor_Control_Input.Vbus);
        SetPWM(Motor_Pin_Map[1], (Outputs + 1), Motor_Control_Input.Vbus);

        Motor_Control_Input.LOAD_OK = 1;
    }
}

void Motor1_DataRead(void* pvParameters) {

    (void)pvParameters;
    volatile FOCInputs* Inputs = Motor_Control_Input.Motor_FOC + 0;
    (void)Inputs;

    //I2C icin DMA Ayari

    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //I2C ile Ia ve Ib okumasi baslatma
        //Encoder Okumasi Baslatma

        //DMA Done Interrupt ile gelecek token i bekleme

        xEventGroupSetBits(xMotorSyncEvents, M1_READY_BIT);
    }

}

void Motor2_DataRead(void* pvParameters) {

    (void)pvParameters;
    volatile FOCInputs* Inputs = Motor_Control_Input.Motor_FOC + 1;
    (void)Inputs;

    //I2C icin DMA Ayari

    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //I2C ile Ia ve Ib okumasi baslatma
        //Encoder Okumasi Baslatma

        //DMA Done Interrupt ile gelecek token i bekleme

        xEventGroupSetBits(xMotorSyncEvents, M2_READY_BIT);
    }

}

static void MotorInit(MotorPinMap* Motor_Pin_Map, uint16_t motor_count) {
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

    PWM3->SM[2].VAL2 = 1500;

    PWM3->SM[2].STS = PWM_STS_RF_MASK;
    PWM3->SM[2].STS = PWM_STS_CMPF(1 << 2);

    PWM3->SM[2].INTEN |= PWM_INTEN_RIE_MASK; //Reload Interrupt Enable
    PWM3->SM[2].INTEN |= PWM_INTEN_CMPIE(1 << 2); //VAL2 Compare Interrupt Enable

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
}

static void ConfigurePWMforMotorRun(uint16_t MotorPin) {
    pwm_info* p = pwm_pin + MotorPin;
    Pins* port = Teensy_Pins + MotorPin;

    IOMUXC->SW_MUX_CTL_PAD[port->mux] = ( IOMUXC->SW_MUX_CTL_PAD[port->mux] & ~(port->mux_mask) ) | p->muxval;

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
}

static uint8_t BitMapControl(MotorPinMap* Motor_Pin_Map, uint16_t motor_count) {
    uint32_t LowPinControl  = 0;
    uint32_t HighPinControl = 0;
    MotorPinMap* pinMap;

    for(int i = 0; i < motor_count; i++) {
        pinMap = Motor_Pin_Map + i;

        //-----MOTOR_1-----
        if(pinMap ->MOTOR_1 > 37) return 0;
        if(pinMap ->MOTOR_1 < 32) {
            if(LowPinControl & (1 << pinMap ->MOTOR_1)) return 0;
            LowPinControl |= 1 << pinMap ->MOTOR_1;
        } else {
            if(HighPinControl & (1 << (pinMap ->MOTOR_1 - 32))) return 0;
            HighPinControl |= 1 << (pinMap ->MOTOR_1 - 32);
        }

        //-----MOTOR_2-----
        if(pinMap ->MOTOR_2 > 37) return 0;
        if(pinMap ->MOTOR_2 < 32) {
            if(LowPinControl & (1 << pinMap ->MOTOR_2)) return 0;
            LowPinControl |= 1 << pinMap ->MOTOR_2;
        } else {
            if(HighPinControl & (1 << (pinMap ->MOTOR_2 - 32))) return 0;
            HighPinControl |= 1 << (pinMap ->MOTOR_2 - 32);
        }

        //-----MOTOR_3-----
        if(pinMap ->MOTOR_3 > 37) return 0;
        if(pinMap ->MOTOR_3 < 32) {
            if(LowPinControl & (1 << pinMap ->MOTOR_3)) return 0;
            LowPinControl |= 1 << pinMap ->MOTOR_3;
        } else {
            if(HighPinControl & (1 << (pinMap ->MOTOR_3 - 32))) return 0;
            HighPinControl |= 1 << (pinMap ->MOTOR_3 - 32);
        }
    }

    return 1;
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

static void CreateLUT() {
    float angle;
    for (int i = 0; i < LUT_SIZE; i++) {
        angle = 2.0f * M_PI * i / LUT_SIZE;
        sin_lookup[i] = sinf(angle);
        cos_lookup[i] = cosf(angle);
    }
}

static void ClarkPark(volatile FOCInputs* Inputs, FOCOutputs* Outputs) {
    float I_alpha = Inputs->Ia;
    float I_beta  = (Inputs->Ia + 2.0f * Inputs->Ib) * ONE_DIVIDED_BY_SQRT_3;
    uint16_t LUT_INDEX = (uint16_t)(Inputs->RotarAngle * LUT_SIZE / ONE_FULL_CIRCLE_ANGLE) & (LUT_SIZE - 1);

    Outputs->Id = I_beta * sin_lookup[LUT_INDEX] + I_alpha * cos_lookup[LUT_INDEX];
    Outputs->Iq = I_beta * cos_lookup[LUT_INDEX] - I_alpha * sin_lookup[LUT_INDEX];
}

static void PIDCalculation(volatile MotorDatas* Motor_Control, volatile FOCInputs* Inputs, FOCOutputs* Outputs, PID* SpeedPID, PID* Iq_PID, PID* Id_PID) {
    if(Motor_Control->FOC_Period_Counter >= FOC_COUNT_PER_SPEED_PID_CYCLE) {
        Inputs->Iq_ref = PID_Compute(SpeedPID, Inputs->Target_Speed, Inputs->Current_Speed, DT_SPEED_PID);
        Motor_Control->FOC_Period_Counter = 0;
    }
    Outputs->Vq = PID_Compute(Iq_PID, Inputs->Iq_ref, Outputs->Iq, DT_FOC);
    Outputs->Vd = PID_Compute(Id_PID, 0.0f, Outputs->Id, DT_FOC);
}

static void InverseClarkPark(volatile FOCInputs* Inputs, FOCOutputs* Outputs) {
    uint16_t LUT_INDEX = (uint16_t)(Inputs->RotarAngle * LUT_SIZE / ONE_FULL_CIRCLE_ANGLE) & (LUT_SIZE - 1);

    float V_alpha = Outputs->Vd*cos_lookup[LUT_INDEX] - Outputs->Vq*sin_lookup[LUT_INDEX];
    float V_beta  = Outputs->Vd*sin_lookup[LUT_INDEX] + Outputs->Vq*cos_lookup[LUT_INDEX];

    Outputs->Va = V_alpha;
    Outputs->Vb = -0.5f * V_alpha + SQRT_3_DIVIDED_BY_TWO * V_beta;
    Outputs->Vc = -0.5f * V_alpha - SQRT_3_DIVIDED_BY_TWO * V_beta;
}

static void SetPWM(MotorPinMap Pins, FOCOutputs* Outputs, float Vbus) {
    float inv_Vbus = 1.0f/Vbus;

    float DutyA = (Outputs->Va + 0.5f*Vbus) * inv_Vbus;
    float DutyB = (Outputs->Vb + 0.5f*Vbus) * inv_Vbus;
    float DutyC = (Outputs->Vc + 0.5f*Vbus) * inv_Vbus;

    ApplyDutyCycle(Pins.MOTOR_1, DutyA);
    ApplyDutyCycle(Pins.MOTOR_2, DutyB);
    ApplyDutyCycle(Pins.MOTOR_3, DutyC);
}

static void ApplyDutyCycle(uint16_t pin, float DutyCycle) {
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
        break;
    case 1:
        flexpwm->SM[submodule].VAL2 = modulo*(1 - DutyCycle);
        break;
    case 2:
        flexpwm->SM[submodule].VAL4 = modulo*(1 - DutyCycle);
    }
}

void PWM3_2_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    uint16_t status = PWM3->SM[2].STS;

    if(status & PWM_STS_RF_MASK) {
        Motor_Control_Input.PMW_Period_Counter = ( Motor_Control_Input.PMW_Period_Counter + 1 ) % PWM_COUNT_PER_FOC_CYCLE;
        if(Motor_Control_Input.PMW_Period_Counter) {
            vTaskNotifyGiveFromISR(Motor2_DataRead_TaskToken, &xHigherPriorityTaskWoken);
            vTaskNotifyGiveFromISR(Motor1_DataRead_TaskToken, &xHigherPriorityTaskWoken);
        }
        PWM3->SM[2].STS = PWM_STS_RF_MASK;
    }

    if(status & PWM_STS_CMPF(1 << 2)) {
        if(Motor_Control_Input.LOAD_OK) {
            PWM1->MCTRL |= PWM_MCTRL_LDOK(0xF);
            PWM2->MCTRL |= PWM_MCTRL_LDOK(0xF);
            PWM3->MCTRL |= PWM_MCTRL_LDOK(0xF);
            PWM4->MCTRL |= PWM_MCTRL_LDOK(0xF);
        } else {
            //Hata
        }
        PWM3->SM[2].STS = PWM_STS_CMPF_MASK;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}