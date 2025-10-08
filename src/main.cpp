#include "MIMXRT1062.h"

#define LED_PIN   3 // GPIO1_IO03

static void delay(volatile uint32_t count) {
    while (count--) __asm("nop");
}

int main(void) {
    /* 1. GPIO1 Clock Enable
       CCM_CCGR1 register'ı, CCM (Clock Controller Module) yapısı içinde tanımlı. */
    CCM->CCGR1 |= CCM_CCGR1_CG13_MASK;  // Enable clock for GPIO1

    /* 2. Pin mux ayarları
       Bu isimler enum (kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03) olarak tanımlanmış.
       IOMUXC->SW_MUX_CTL_PAD[] dizisini kullanmamız gerekiyor. */
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03] = 5;    // ALT5 = GPIO1_IO03
    IOMUXC->SW_PAD_CTL_PAD[kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_03] = 0x10B0; // Drive strength config

    /* 3. GPIO yönü çıkış olarak ayarla */
    GPIO1->GDIR |= (1 << LED_PIN);

    /* 4. Sonsuz döngüde LED yak/söndür */
    while (1) {
        GPIO1->DR_SET = (1 << LED_PIN);   // LED ON
        delay(10000000);
        GPIO1->DR_CLEAR = (1 << LED_PIN); // LED OFF
        delay(10000000);
    }
}
