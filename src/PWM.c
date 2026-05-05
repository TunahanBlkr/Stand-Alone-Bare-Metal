#include "PWM.h"

#define M(a, b) ((((a) - 1) << 2) | (b))

pwm_info pwm_pin[55] = {
	{1, M(1, 1), 0, 4},  // FlexPWM1_1_X   0  // AD_B0_03
	{1, M(1, 0), 0, 4},  // FlexPWM1_0_X   1  // AD_B0_02
	{1, M(4, 2), 1, 1},  // FlexPWM4_2_A   2  // EMC_04
	{1, M(4, 2), 2, 1},  // FlexPWM4_2_B   3  // EMC_05
	{1, M(2, 0), 1, 1},  // FlexPWM2_0_A   4  // EMC_06
	{1, M(2, 1), 1, 1},  // FlexPWM2_1_A   5  // EMC_08
	{1, M(2, 2), 1, 2},  // FlexPWM2_2_A   6  // B0_10
	{1, M(1, 3), 2, 6},  // FlexPWM1_3_B   7  // B1_01
	{1, M(1, 3), 1, 6},  // FlexPWM1_3_A   8  // B1_00
	{1, M(2, 2), 2, 2},  // FlexPWM2_2_B   9  // B0_11
	{2, M(1, 0), 0, 1},  // QuadTimer1_0  10  // B0_00
	{2, M(1, 2), 0, 1},  // QuadTimer1_2  11  // B0_02
	{2, M(1, 1), 0, 1},  // QuadTimer1_1  12  // B0_01
	{2, M(2, 0), 0, 1},  // QuadTimer2_0  13  // B0_03
	{2, M(3, 2), 0, 1},  // QuadTimer3_2  14  // AD_B1_02
	{2, M(3, 3), 0, 1},  // QuadTimer3_3  15  // AD_B1_03
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{2, M(3, 1), 0, 1},  // QuadTimer3_1  18  // AD_B1_01
	{2, M(3, 0), 0, 1},  // QuadTimer3_0  19  // AD_B1_00
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{1, M(4, 0), 1, 1},  // FlexPWM4_0_A  22  // AD_B1_08
	{1, M(4, 1), 1, 1},  // FlexPWM4_1_A  23  // AD_B1_09
	{1, M(1, 2), 0, 4},  // FlexPWM1_2_X  24  // AD_B0_12
	{1, M(1, 3), 0, 4},  // FlexPWM1_3_X  25  // AD_B0_13
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{1, M(3, 1), 2, 1},  // FlexPWM3_1_B  28  // EMC_32
	{1, M(3, 1), 1, 1},  // FlexPWM3_1_A  29  // EMC_31
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{1, M(2, 0), 2, 1},  // FlexPWM2_0_B  33  // EMC_07
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{1, M(2, 3), 1, 6},  // FlexPWM2_3_A  36  // B1_00
	{1, M(2, 3), 2, 6},  // FlexPWM2_3_B  37  // B1_01
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{0, M(1, 0), 0, 0},
	{1, M(1, 1), 2, 1},  // FlexPWM1_1_B  42  // SD_B0_03
	{1, M(1, 1), 1, 1},  // FlexPWM1_1_A  43  // SD_B0_02
	{1, M(1, 0), 2, 1},  // FlexPWM1_0_B  44  // SD_B0_01
	{1, M(1, 0), 1, 1},  // FlexPWM1_0_A  45  // SD_B0_00
	{1, M(1, 2), 2, 1},  // FlexPWM1_2_B  46  // SD_B0_05
	{1, M(1, 2), 1, 1},  // FlexPWM1_2_A  47  // SD_B0_04
	{0, M(1, 0), 0, 0},  // duplicate FlexPWM1_0_B
	{0, M(1, 0), 0, 0},  // duplicate FlexPWM1_2_A
	{0, M(1, 0), 0, 0},  // duplicate FlexPWM1_2_B
	{1, M(3, 3), 2, 1},  // FlexPWM3_3_B  51  // EMC_22
	{0, M(1, 0), 0, 0},  // duplicate FlexPWM1_1_B
	{0, M(1, 0), 0, 0},  // duplicate FlexPWM1_1_A
	{1, M(3, 0), 1, 1},  // FlexPWM3_0_A  54  // EMC_29
};

void analogWrite(uint8_t pin, uint16_t val) {

    if(pin>=55) return;

    pwm_info* p = pwm_pin + pin;

	if(p->type != 1) return;

	PWM_Type* flexpwm;

	switch ((p->module >> 2) & 3) {
		case 0:  flexpwm = PWM1; break;
		case 1:  flexpwm = PWM2; break;
		case 2:  flexpwm = PWM3; break;
		default: flexpwm = PWM4;
	}

	uint8_t  submodule = p->module & 3;
	uint16_t mask      = 1 << submodule;
	uint32_t modulo    = flexpwm->SM[submodule].VAL1;
	uint32_t cval      = ((uint32_t)val * (modulo + 1)) >> 8;

	if (cval > modulo) cval = modulo;

	flexpwm->MCTRL |= PWM_MCTRL_CLDOK(mask);
	switch (p->channel) {
	case 0: // X
		// Her zaman right aligned
		flexpwm->SM[submodule].VAL0 = modulo - cval;
		flexpwm->OUTEN |= PWM_OUTEN_PWMX_EN(mask);
		break;
	case 1: // A
		switch(PWM_ALIGN_TYPE) {
		case LEFT_ALIGNED:
			flexpwm->SM[submodule].VAL2 =  0;
			flexpwm->SM[submodule].VAL3 =  cval;
			break;
		case CENTER:
			flexpwm->SM[submodule].VAL2 = -cval;
			flexpwm->SM[submodule].VAL3 =  cval;
			break;
		case RIGHT_ALIGNED:
			flexpwm->SM[submodule].VAL2 =  modulo - cval;
			flexpwm->SM[submodule].VAL3 =  modulo;
		}
		flexpwm->OUTEN |= PWM_OUTEN_PWMA_EN(mask);
		break;
	case 2: // B
		switch(PWM_ALIGN_TYPE) {
		case LEFT_ALIGNED:
			flexpwm->SM[submodule].VAL4 =  0;
			flexpwm->SM[submodule].VAL5 =  cval;
			break;
		case CENTER:
			flexpwm->SM[submodule].VAL4 = -cval;
			flexpwm->SM[submodule].VAL5 =  cval;
			break;
		case RIGHT_ALIGNED:
			flexpwm->SM[submodule].VAL4 =  modulo - cval;
			flexpwm->SM[submodule].VAL5 =  modulo;
		}
		flexpwm->OUTEN |= PWM_OUTEN_PWMB_EN(mask);
	}
	flexpwm->MCTRL |= PWM_MCTRL_LDOK(mask);

	Pins* port = Teensy_Pins + pin;

	IOMUXC->SW_MUX_CTL_PAD[port->mux] = ( IOMUXC->SW_MUX_CTL_PAD[port->mux] & ~(port->mux_mask) ) | p->muxval;
	//Pad ayarlarini yap
}

static uint16_t find_pwm_period(uint32_t clock, uint32_t frequency){
	if(frequency == 0) return 0;

	uint32_t divider = clock / frequency;
	uint16_t prescaler = 0;

	while((divider >> prescaler) >= 65535) {
		prescaler++;
	}

	return divider >> prescaler;
};

static void flexpwm_init(PWM_Type* pwm) {
	uint32_t period = find_pwm_period(PWM_CLK_HZ, PWM_FREQUENCY);
	uint16_t prescaler = 0;

	if(!period) {/*Hata*/};

	while((period << prescaler) < (PWM_CLK_HZ/PWM_FREQUENCY))
		prescaler++;
	
	pwm->FCTRL  = PWM_FCTRL_FLVL(15);
	pwm->FSTS   = PWM_FSTS_FFLAG(15);

	pwm->MCTRL |= PWM_MCTRL_CLDOK(15);

	for(int i= 0; i<4; i++) {
		pwm->SM[i].CTRL2     =  PWM_CTRL2_INDEP_MASK;
		pwm->SM[i].CTRL      =  PWM_CTRL_FULL_MASK | PWM_CTRL_PRSC(prescaler);
		pwm->SM[i].OCTRL     =  0;
		pwm->SM[i].DTCNT0    =  0;
		pwm->SM[i].DTCNT1    =  0;
		switch(PWM_ALIGN_TYPE) {
		case LEFT_ALIGNED:
			pwm->SM[i].INIT  =  0;
			pwm->SM[i].VAL0  =  period;
			pwm->SM[i].VAL1  =  period;
			pwm->SM[i].VAL2  =  0;
			pwm->SM[i].VAL3  =  0;
			pwm->SM[i].VAL4  =  0;
			pwm->SM[i].VAL5  =  0;
			break;
		case CENTER:
			pwm->SM[i].INIT  = -period/2;
			pwm->SM[i].VAL0  =  period/2;
			pwm->SM[i].VAL1  =  period/2;
			pwm->SM[i].VAL2  =  0;
			pwm->SM[i].VAL3  =  0;
			pwm->SM[i].VAL4  =  0;
			pwm->SM[i].VAL5  =  0;
			break;
		case RIGHT_ALIGNED:
			pwm->SM[i].INIT  =  0;
			pwm->SM[i].VAL0  =  period;
			pwm->SM[i].VAL1  =  period;
			pwm->SM[i].VAL2  =  period;
			pwm->SM[i].VAL3  =  period;
			pwm->SM[i].VAL4  =  period;
			pwm->SM[i].VAL5  =  period;
		}
	}

	pwm->MCTRL |= PWM_MCTRL_LDOK(15);
	pwm->MCTRL |= PWM_MCTRL_RUN(15);

}

void pwm_init(void) {

	CCM->CCGR4 |= CCM_CCGR4_CG8_MASK | CCM_CCGR4_CG9_MASK | CCM_CCGR4_CG10_MASK | CCM_CCGR4_CG11_MASK;  //FlexPWM Clk Init

	flexpwm_init(PWM1);
	flexpwm_init(PWM2);
	flexpwm_init(PWM3);
	flexpwm_init(PWM4);

}