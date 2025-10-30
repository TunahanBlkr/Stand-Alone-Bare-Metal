#include "Teensy_UART.h"

// Test stringler
const char *test_str1 = "Hello from LPUART1!\r\n";
const char *test_str2 = "Hello from LPUART2!\r\n";

void LPUART1_Task(void *pvParameters) {
    
    (void)pvParameters;
    char c;

    while(1) {
        
        LPUART1_Write_String(test_str1);
        while ((c = LPUART1_Read_Char()) != 0)
        {
            LPUART1_Write_Char(c);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void LPUART2_Task(void *pvParameters) {
    
    (void)pvParameters;
    char c;

    while(1) {
        
        LPUART2_Write_String(test_str2);
        while ((c = LPUART2_Read_Char()) != 0)
        {
            LPUART2_Write_Char(c);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {

    // LPUART Init
    LPUART1_INIT(115200);
    LPUART2_INIT(115200);

    // FreeRTOS Tasks
    xTaskCreate(LPUART1_Task, "LPUART1", 256, NULL, 2, NULL);
    xTaskCreate(LPUART2_Task, "LPUART2", 256, NULL, 2, NULL);

    // FreeRTOS scheduler Baslat
    vTaskStartScheduler();

    while(1);
    return 0;
}
