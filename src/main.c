#include "MIMXRT1062.h"
#include "Teensy_Pins.h"
#include "FreeRTOS.h"
#include "task.h"

static void vBlink13Task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        PIN13_GPIO->DR_TOGGLE = PIN13_BITMASK;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void vBlink9Task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        PIN9_GPIO->DR_TOGGLE = PIN9_BITMASK;
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

int main(void) {

    CCM->CCGR0 |= CCM_CCGR0_CG15_MASK;

    IOMUXC->SW_MUX_CTL_PAD[PIN13_MUX] = ( IOMUXC->SW_MUX_CTL_PAD[PIN13_MUX] & ~0xF ) | 0x5;
    IOMUXC->SW_MUX_CTL_PAD[PIN9_MUX] = ( IOMUXC->SW_MUX_CTL_PAD[PIN9_MUX] & ~0xF ) | 0x5;

    PIN13_GPIO->GDIR |= PIN13_BITMASK;
    PIN9_GPIO->GDIR |= PIN9_BITMASK;

    xTaskCreate( vBlink13Task, "LED13", 256, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( vBlink9Task, "LED9", 256, NULL, tskIDLE_PRIORITY + 1, NULL );

    vTaskStartScheduler();

    /* Buraya gelinirse heap veya config hatası vardır */
    while (1) {}
}
