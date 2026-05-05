#include "UART.h"

QueueHandle_t SERIAL1_TX_QUEUE;
QueueHandle_t SERIAL1_RX_QUEUE;

QueueHandle_t SERIAL2_TX_QUEUE;
QueueHandle_t SERIAL2_RX_QUEUE;

QueueHandle_t SERIAL3_TX_QUEUE;
QueueHandle_t SERIAL3_RX_QUEUE;

QueueHandle_t SERIAL4_TX_QUEUE;
QueueHandle_t SERIAL4_RX_QUEUE;

QueueHandle_t SERIAL5_TX_QUEUE;
QueueHandle_t SERIAL5_RX_QUEUE;

QueueHandle_t SERIAL6_TX_QUEUE;
QueueHandle_t SERIAL6_RX_QUEUE;

QueueHandle_t SERIAL7_TX_QUEUE;
QueueHandle_t SERIAL7_RX_QUEUE;

QueueHandle_t SERIAL8_TX_QUEUE;
QueueHandle_t SERIAL8_RX_QUEUE;

static void Serial_Common_Init(LPUART_Type* SERIAL, uint32_t baudrate) {
    SERIAL->GLOBAL |= LPUART_GLOBAL_RST_MASK; // UART Reset
    SERIAL->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    SERIAL->CTRL = 0; // 8-bit, no parity, Module Disable

    SERIAL->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK); // TX/RX WATER Settings
    SERIAL->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    SERIAL->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK); // TX/RX FIFO Enable

    uint16_t sbr = (UART_CLK_FREQ / (16U * baudrate)); // Baud rate Settings
    SERIAL->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL->BAUD |= LPUART_BAUD_SBR(sbr);

    SERIAL->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serialx_Write_Char sonrasi acilacak

    // TX ve RX baska bir yerde aktif edilecek
}

/********************************************************************************************************************************/
/*                                                        SERIAL1                                                               */
/********************************************************************************************************************************/

void Serial1_Init(uint32_t baudrate) {
    CCM->CCGR3 |= CCM_CCGR3_CG3_MASK; // LPUART6 clock enable

    Serial_Common_Init(SERIAL1, baudrate);

    SERIAL1_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL1_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL1_IRQn, 5); // NVIC interrupt enable (IRQn LPUART6)
    NVIC_EnableIRQ(SERIAL1_IRQn);

    // SERIAL1_TX = PIN1, SERIAL1_RX = PIN0
    IOMUXC->SW_MUX_CTL_PAD[PIN1_MUX] = 0x2; // ALT2 = LPUART6_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN0_MUX] = 0x2; // ALT2 = LPUART6_RX

    SERIAL1->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial1_Write_Char(char c) {
    xQueueSend(SERIAL1_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL1->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial1_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL1_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL1->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial1_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL1_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                        SERIAL2                                                               */
/********************************************************************************************************************************/

void Serial2_Init(uint32_t baudrate) {
    CCM->CCGR1 |= CCM_CCGR1_CG12_MASK; // LPUART4 clock enable

    Serial_Common_Init(SERIAL2, baudrate);

    SERIAL2_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL2_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL2_IRQn, 5); // NVIC interrupt enable (IRQn LPUART4)
    NVIC_EnableIRQ(SERIAL2_IRQn);

    // SERIAL2_TX = PIN8, SERIAL2_RX = PIN7
    IOMUXC->SW_MUX_CTL_PAD[PIN8_MUX] = 0x2; // ALT2 = LPUART4_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN7_MUX] = 0x2; // ALT2 = LPUART4_RX

    SERIAL2->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial2_Write_Char(char c) {
    xQueueSend(SERIAL2_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL2->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial2_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL2_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL2->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial2_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL2_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                        SERIAL3                                                               */
/********************************************************************************************************************************/

void Serial3_Init(uint32_t baudrate) {
    CCM->CCGR0 |= CCM_CCGR0_CG14_MASK; // LPUART2 clock enable

    Serial_Common_Init(SERIAL3, baudrate);

    SERIAL3_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL3_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL3_IRQn, 5); // NVIC interrupt enable (IRQn LPUART2)
    NVIC_EnableIRQ(SERIAL3_IRQn);

    // SERIAL3_TX = PIN14, SERIAL3_RX = PIN15
    IOMUXC->SW_MUX_CTL_PAD[PIN14_MUX] = 0x2; // ALT2 = LPUART2_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN15_MUX] = 0x2; // ALT2 = LPUART2_RX

    SERIAL3->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial3_Write_Char(char c) {
    xQueueSend(SERIAL3_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL3->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial3_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL3_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL3->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial3_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL3_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                        SERIAL4                                                               */
/********************************************************************************************************************************/

void Serial4_Init(uint32_t baudrate) {
    CCM->CCGR0 |= CCM_CCGR0_CG6_MASK; // LPUART3 clock enable

    Serial_Common_Init(SERIAL4, baudrate);

    SERIAL4_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL4_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL4_IRQn, 5); // NVIC interrupt enable (IRQn LPUART3)
    NVIC_EnableIRQ(SERIAL4_IRQn);

    // SERIAL4_TX = PIN17, SERIAL4_RX = PIN16
    IOMUXC->SW_MUX_CTL_PAD[PIN17_MUX] = 0x2; // ALT2 = LPUART3_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN16_MUX] = 0x2; // ALT2 = LPUART3_RX

    SERIAL4->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial4_Write_Char(char c) {
    xQueueSend(SERIAL4_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL4->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial4_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL4_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL4->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial4_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL4_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                        SERIAL5                                                               */
/********************************************************************************************************************************/

void Serial5_Init(uint32_t baudrate) {
    CCM->CCGR6 |= CCM_CCGR6_CG7_MASK; // LPUART8 clock enable

    Serial_Common_Init(SERIAL5, baudrate);

    SERIAL5_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL5_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL5_IRQn, 5); // NVIC interrupt enable (IRQn LPUART8)
    NVIC_EnableIRQ(SERIAL5_IRQn);

    // SERIAL5_TX = PIN20, SERIAL5_RX = PIN21
    IOMUXC->SW_MUX_CTL_PAD[PIN20_MUX] = 0x2; // ALT2 = LPUART8_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN21_MUX] = 0x2; // ALT2 = LPUART8_RX

    SERIAL5->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial5_Write_Char(char c) {
    xQueueSend(SERIAL5_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL5->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial5_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL5_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL5->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial5_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL5_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                        SERIAL6                                                               */
/********************************************************************************************************************************/

void Serial6_Init(uint32_t baudrate) {
    CCM->CCGR5 |= CCM_CCGR5_CG12_MASK; // LPUART1 clock enable

    Serial_Common_Init(SERIAL6, baudrate);

    SERIAL6_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL6_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL6_IRQn, 5); // NVIC interrupt enable (IRQn LPUART1)
    NVIC_EnableIRQ(SERIAL6_IRQn);

    // SERIAL6_TX = PIN24, SERIAL6_RX = PIN25
    IOMUXC->SW_MUX_CTL_PAD[PIN24_MUX] = 0x2; // ALT2 = LPUART1_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN25_MUX] = 0x2; // ALT2 = LPUART1_RX

    SERIAL6->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial6_Write_Char(char c) {
    xQueueSend(SERIAL6_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL6->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial6_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL6_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL6->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial6_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL6_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                        SERIAL7                                                               */
/********************************************************************************************************************************/

void Serial7_Init(uint32_t baudrate) {
    CCM->CCGR5 |= CCM_CCGR5_CG13_MASK; // LPUART7 clock enable

    Serial_Common_Init(SERIAL7, baudrate);

    SERIAL7_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL7_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL7_IRQn, 5); // NVIC interrupt enable (IRQn LPUART7)
    NVIC_EnableIRQ(SERIAL7_IRQn);

    // SERIAL7_TX = GPIO_EMC_31, SERIAL7_RX = GPIO_EMC_32
    IOMUXC->SW_MUX_CTL_PAD[PIN29_MUX] = 0x2; // ALT2 = LPUART7_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN28_MUX] = 0x2; // ALT2 = LPUART7_RX

    SERIAL7->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial7_Write_Char(char c) {
    xQueueSend(SERIAL7_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL7->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial7_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL7_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL7->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial7_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL7_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                        SERIAL8                                                               */
/********************************************************************************************************************************/

void Serial8_Init(uint32_t baudrate) {
    CCM->CCGR3 |= CCM_CCGR3_CG1_MASK; // LPUART5 clock enable

    Serial_Common_Init(SERIAL8, baudrate);

    SERIAL8_TX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));
    SERIAL8_RX_QUEUE = xQueueCreate(UART_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(SERIAL8_IRQn, 5); // NVIC interrupt enable (IRQn LPUART5)
    NVIC_EnableIRQ(SERIAL8_IRQn);

    // SERIAL8_TX = PIN35, SERIAL8_RX = PIN34
    IOMUXC->SW_MUX_CTL_PAD[PIN35_MUX] = 0x1; // ALT1 = LPUART5_TX
    IOMUXC->SW_MUX_CTL_PAD[PIN34_MUX] = 0x1; // ALT1 = LPUART5_RX

    SERIAL8->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
}

void Serial8_Write_Char(char c) {
    xQueueSend(SERIAL8_TX_QUEUE, &c, portMAX_DELAY);
    SERIAL8->CTRL |= LPUART_CTRL_TIE_MASK;
}

void Serial8_Write_String(const char *str) {
    while (*str) {
        xQueueSend(SERIAL8_TX_QUEUE, str++, portMAX_DELAY);
    }
    SERIAL8->CTRL |= LPUART_CTRL_TIE_MASK;
}

char Serial8_Read_Char(void) {
    char c;
    xQueueReceive(SERIAL8_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

/********************************************************************************************************************************/
/*                                                      IRQHandlers                                                             */
/********************************************************************************************************************************/

void LPUART1_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL6->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL6);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL6_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL6->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL6->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL6->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL6->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL6->DATA & 0xFF);
            xQueueSendFromISR(SERIAL6_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART2_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL3->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL3);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL3_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL3->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL3->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL3->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL3->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL3->DATA & 0xFF);
            xQueueSendFromISR(SERIAL3_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART3_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL4->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL4);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL4_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL4->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL4->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL4->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL4->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL4->DATA & 0xFF);
            xQueueSendFromISR(SERIAL4_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART4_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL2->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL2);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL2_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL2->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL2->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL2->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL2->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL2->DATA & 0xFF);
            xQueueSendFromISR(SERIAL2_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART5_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL8->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL8);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL8_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL8->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL8->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL8->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL8->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL8->DATA & 0xFF);
            xQueueSendFromISR(SERIAL8_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART6_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL1->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL1);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL1_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL1->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL1->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL1->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL1->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL1->DATA & 0xFF);
            xQueueSendFromISR(SERIAL1_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART7_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL7->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL7);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL7_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL7->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL7->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL7->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL7->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL7->DATA & 0xFF);
            xQueueSendFromISR(SERIAL7_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART8_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL5->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = UART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL5);
        char c;
        while (space--) {
            if (xQueueReceiveFromISR(SERIAL5_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                SERIAL5->DATA = c; // FIFO’ya yaz
            } else {
                SERIAL5->CTRL &= ~LPUART_CTRL_TIE_MASK; // Queue bos → TIE disable
                break; // FIFO dolu olmayabilir.
            }
        }
    }

    // RX interrupt
    if(SERIAL5->STAT & LPUART_STAT_RDRF_MASK) {
        while(!(SERIAL5->DATA & LPUART_DATA_RXEMPT_MASK)) {
            char c = (char)(SERIAL5->DATA & 0xFF);
            xQueueSendFromISR(SERIAL5_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}