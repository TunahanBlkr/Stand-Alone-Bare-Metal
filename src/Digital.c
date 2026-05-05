#include "Digital.h"

void pinMode(uint8_t pin, PIN_MODE mode) {
    
    if(pin>=55) return;

    Pins* p = Teensy_Pins + pin;

    if(mode == OUTPUT || mode == OUTPUT_OPENDRAIN) {

        p->gpio->GDIR |= (1 << p->bit);

        if(mode == OUTPUT) {
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6);
        } else { // OUTPUT_OPENDRAIN
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_ODE_MASK;
        }

    } else {

        p->gpio->GDIR &= ~(1 << p->bit);
        
        if(mode == INPUT) {
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6);
        } else if(mode == INPUT_PULLUP) {
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(3) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK;
        } else if(mode == INPUT_PULLDOWN) {
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(0) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK;
        } else { // INPUT_DISABLE
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK;
        }
        
    }

    IOMUXC->SW_MUX_CTL_PAD[p->mux] = ( IOMUXC->SW_MUX_CTL_PAD[p->mux] & ~(p->mux_mask) ) | 0x5;
}

void digitalToggle(uint8_t pin) {
    // Bu fonksiyon olabildigince kullanilmamali.
    if(pin>=55) return;

    Pins* p = Teensy_Pins + pin;

    p->gpio->DR_TOGGLE  = 1 << p->bit;
}

void digitalWrite(uint8_t pin, uint8_t val) {
    // Bu fonksiyon olabildigince kullanilmamali.
    if(pin>=55) return;

    Pins* p = Teensy_Pins + pin;

    if(p->gpio->GDIR & (1 << p->bit)) {
        if(val) {
            p->gpio->DR_SET   = 1 << p->bit;
        } else {
            p->gpio->DR_CLEAR = 1 << p->bit;
        }
    } else {
        if(val) {
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(3) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK;
        } else {
            IOMUXC->SW_PAD_CTL_PAD[p->pad] = IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(0) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK;
        }
    }
}

bool digitalRead(uint8_t pin) {
    // Bu fonksiyon olabildigince kullanilmamali.
    if(pin>=55) return 0;

    Pins* p = Teensy_Pins + pin;

    return (p->gpio->PSR & (1 << p->bit)) ? true : false;
}