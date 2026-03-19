#include "MIMXRT1062.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Teensy_Pins.h"
#include "Teensy_Digital.h"
#include "Teensy_PWM.h"
#include "Teensy_Motor_Controller.h"


static void vBlink13Task(void *pvParameters)
{
    (void)pvParameters;

    while (1)
    {
        //digitalWrite(13, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {

    CCM->CCGR0 |= CCM_CCGR0_CG15_MASK;

    pinMode(13, OUTPUT);

    xTaskCreate( vBlink13Task, "LED13", 256, NULL, 2, NULL );
    xTaskCreate( MotorHandler, "Motor_H", 512, NULL, configMAX_PRIORITIES - 2, NULL);
    xTaskCreate( Motor1_DataRead, "M1_DR", 512, NULL, configMAX_PRIORITIES - 1, &Motor1_DataRead_TaskToken);
    xTaskCreate( Motor2_DataRead, "M2_DR", 512, NULL, configMAX_PRIORITIES - 1, &Motor2_DataRead_TaskToken);

    vTaskStartScheduler();

    /* Buraya gelinirse heap veya config hatası vardır */
    while (1) {}
}
