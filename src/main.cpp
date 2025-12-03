#include "Teensy_UART.h"

// Test stringler
const char *test_str1 = "Hello from Serial1!\r\n";
const char *test_str2 = "Hello from Serial2!\r\n";

void Serial1_Task(void *pvParameters) {
    
    (void)pvParameters;
    char c;

    while(1) {
        
        Serial1_Write_String(test_str1);
        while ((c = Serial1_Read_Char()) != 0)
        {
            Serial1_Write_Char(c);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void Serial2_Task(void *pvParameters) {
    
    (void)pvParameters;
    char c;

    while(1) {
        
        Serial2_Write_String(test_str2);
        while ((c = Serial2_Read_Char()) != 0)
        {
            Serial2_Write_Char(c);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {

    // Serial Init
    Serial1_Init(115200);
    Serial2_Init(115200);

    // FreeRTOS Tasks
    xTaskCreate(Serial1_Task, "Serial1", 256, NULL, 2, NULL);
    xTaskCreate(Serial2_Task, "Serial2", 256, NULL, 2, NULL);

    // FreeRTOS scheduler Baslat
    vTaskStartScheduler();

    while(1);
    return 0;
}
