#include "Teensy_UART.h"

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

uint8_t LPUART_FIFO_SIZE = 0;

/********************************************************************************************************************************/
/*                                                        SERIAL1                                                               */
/********************************************************************************************************************************/

void Serial1_Init(uint32_t baudrate) {
    // LPUART saat ayari (CCM)
    CCM->CCGR3 |= CCM_CCGR3_CG3_MASK; // LPUART6 clock enable

    // Pin mux ayarları
    // SERIAL1_TX = GPIO_AD_B0_02, SERIAL1_RX = GPIO_AD_B0_03
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_02] &= ~0x7;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_02] |= 0x2; // ALT2 = LPUART6_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_03] &= ~0x7;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_03] |= 0x2; // ALT2 = LPUART6_RX

    // UART reset
    SERIAL1->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL1->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL1->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL1->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL1->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL1->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL1->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL1->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL1->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL1->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL1->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL1->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL1->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial1_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL1_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL1_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART6_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART6)
    NVIC_EnableIRQ(LPUART6_IRQn);

    // Modulu ac
    SERIAL1->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
    // LPUART saat ayari (CCM)
    CCM->CCGR1 |= CCM_CCGR1_CG12_MASK; // LPUART4 clock enable

    // Pin mux ayarları
    // SERIAL2_TX = GPIO_B1_00, SERIAL2_RX = GPIO_B1_01
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_00] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_00] |= 0x2; // ALT2 = LPUART4_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_01] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_01] |= 0x2; // ALT2 = LPUART4_RX

    // UART reset
    SERIAL2->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL2->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL2->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL2->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL2->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL2->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL2->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL2->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL2->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL2->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL2->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL2->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL2->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial2_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL2_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL2_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART4_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART4)
    NVIC_EnableIRQ(LPUART4_IRQn);

    // Modulu ac
    SERIAL2->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
    // LPUART saat ayari (CCM)
    CCM->CCGR0 |= CCM_CCGR0_CG14_MASK; // LPUART2 clock enable

    // Pin mux ayarları
    // SERIAL3_TX = GPIO_AD_B1_02, SERIAL3_RX = GPIO_AD_B1_03
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_02] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_02] |= 0x2; // ALT2 = LPUART2_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_03] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_03] |= 0x2; // ALT2 = LPUART2_RX

    // UART reset
    SERIAL3->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL3->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL3->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL3->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL3->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL3->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL3->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL3->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL3->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL3->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL3->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL3->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL3->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial3_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL3_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL3_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART2_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART2)
    NVIC_EnableIRQ(LPUART2_IRQn);

    // Modulu ac
    SERIAL3->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
    // LPUART saat ayari (CCM)
    CCM->CCGR0 |= CCM_CCGR0_CG6_MASK; // LPUART3 clock enable

    // Pin mux ayarları
    // SERIAL4_TX = GPIO_AD_B1_06, SERIAL4_RX = GPIO_AD_B1_07
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_06] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_06] |= 0x2; // ALT2 = LPUART3_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_07] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_07] |= 0x2; // ALT2 = LPUART3_RX

    // UART reset
    SERIAL4->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL4->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL4->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL4->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL4->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL4->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL4->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL4->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL4->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL4->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL4->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL4->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL4->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial4_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL4_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL4_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART3_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART3)
    NVIC_EnableIRQ(LPUART3_IRQn);

    // Modulu ac
    SERIAL4->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
    // LPUART saat ayari (CCM)
    CCM->CCGR6 |= CCM_CCGR6_CG7_MASK; // LPUART8 clock enable

    // Pin mux ayarları
    // SERIAL5_TX = GPIO_AD_B1_10, SERIAL5_RX = GPIO_AD_B1_11
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_10] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_10] |= 0x2; // ALT2 = LPUART8_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_11] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_11] |= 0x2; // ALT2 = LPUART8_RX

    // UART reset
    SERIAL5->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL5->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL5->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL5->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL5->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL5->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL5->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL5->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL5->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL5->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL5->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL5->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL5->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial5_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL5_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL5_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART8_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART8)
    NVIC_EnableIRQ(LPUART8_IRQn);

    // Modulu ac
    SERIAL5->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
    // LPUART saat ayari (CCM)
    CCM->CCGR5 |= CCM_CCGR5_CG12_MASK; // LPUART1 clock enable

    // Pin mux ayarları
    // SERIAL6_TX = GPIO_AD_B0_12, SERIAL6_RX = GPIO_AD_B0_13
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_12] &= ~0x7;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_12] |= 0x2; // ALT2 = LPUART1_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_13] &= ~0x7;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_13] |= 0x2; // ALT2 = LPUART1_RX

    // UART reset
    SERIAL6->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL6->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL6->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL6->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL6->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL6->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL6->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL6->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL6->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL6->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL6->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL6->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL6->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial6_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL6_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL6_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART1_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART1)
    NVIC_EnableIRQ(LPUART1_IRQn);

    // Modulu ac
    SERIAL6->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
    // LPUART saat ayari (CCM)
    CCM->CCGR5 |= CCM_CCGR5_CG13_MASK; // LPUART7 clock enable

    // Pin mux ayarları
    // SERIAL7_TX = GPIO_EMC_31, SERIAL7_RX = GPIO_EMC_32
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_31] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_31] |= 0x2; // ALT2 = LPUART7_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32] |= 0x2; // ALT2 = LPUART7_RX

    // UART reset
    SERIAL7->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL7->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL7->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL7->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL7->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL7->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL7->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL7->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL7->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL7->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL7->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL7->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL7->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial7_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL7_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL7_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART7_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART7)
    NVIC_EnableIRQ(LPUART7_IRQn);

    // Modulu ac
    SERIAL7->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
    // LPUART saat ayari (CCM)
    CCM->CCGR3 |= CCM_CCGR3_CG1_MASK; // LPUART5 clock enable

    // Pin mux ayarları
    // SERIAL8_TX = GPIO_B1_12, SERIAL8_RX = GPIO_B1_13
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_12] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_12] |= 0x1; // ALT1 = LPUART5_TX

    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_13] &= ~0xF;
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_13] |= 0x1; // ALT1 = LPUART5_RX

    // UART reset
    SERIAL8->GLOBAL |= LPUART_GLOBAL_RST_MASK;
    SERIAL8->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

    // Modulu kapat
    SERIAL8->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL8->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    SERIAL8->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);

    SERIAL8->WATER &= ~(LPUART_WATER_TXWATER_MASK | LPUART_WATER_RXWATER_MASK);
    SERIAL8->WATER |= LPUART_WATER_TXWATER(2U) | LPUART_WATER_RXWATER(0U);

    if(!(LPUART_FIFO_SIZE))
        LPUART_FIFO_SIZE = 1U << (((SERIAL8->FIFO & LPUART_FIFO_TXFIFOSIZE_MASK) >> LPUART_FIFO_TXFIFOSIZE_SHIFT) + 1U);

    // Baud rate ayari
    uint32_t sbr = (UART_CLK_FREQ / (16U * baudrate));
    SERIAL8->BAUD &= ~LPUART_BAUD_SBR_MASK;
    SERIAL8->BAUD |= LPUART_BAUD_SBR(sbr);

    // 8-bit, no parity
    SERIAL8->CTRL = 0; // UART reset atinca sifirlandi zaten. Bu satir sadece okunabilirlik daha kolay olsun diye var.

    SERIAL8->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK); // TX ve RX enable
    SERIAL8->CTRL |= LPUART_CTRL_RIE_MASK; // RX interrupt enable - TX kesmesi Serial8_Write_Char sonrasi acilacak

    // FreeRTOS queue oluştur
    SERIAL8_TX_QUEUE = xQueueCreate(UART_TX_BUFFER_SIZE, sizeof(char));
    SERIAL8_RX_QUEUE = xQueueCreate(UART_RX_BUFFER_SIZE, sizeof(char));

    NVIC_SetPriority(LPUART5_IRQn, 5);

    // NVIC interrupt enable (IRQn LPUART5)
    NVIC_EnableIRQ(LPUART5_IRQn);

    // Modulu ac
    SERIAL8->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
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
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL6);
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
    while (SERIAL6->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL6->DATA & 0xFF);
        xQueueSendFromISR(SERIAL6_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART2_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL3->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL3);
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
    while (SERIAL3->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL3->DATA & 0xFF);
        xQueueSendFromISR(SERIAL3_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART3_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL4->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL4);
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
    while (SERIAL4->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL4->DATA & 0xFF);
        xQueueSendFromISR(SERIAL4_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART4_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL2->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL2);
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
    while (SERIAL2->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL2->DATA & 0xFF);
        xQueueSendFromISR(SERIAL2_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART5_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL8->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL8);
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
    while (SERIAL8->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL8->DATA & 0xFF);
        xQueueSendFromISR(SERIAL8_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART6_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL1->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL1);
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
    while (SERIAL1->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL1->DATA & 0xFF);
        xQueueSendFromISR(SERIAL1_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART7_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL7->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL7);
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
    while (SERIAL7->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL7->DATA & 0xFF);
        xQueueSendFromISR(SERIAL7_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void LPUART8_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // TX interrupt
    if (SERIAL5->STAT & LPUART_STAT_TDRE_MASK) {
        uint8_t space = LPUART_FIFO_SIZE - LPUART_TXCOUNT(SERIAL5);
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
    while (SERIAL5->STAT & LPUART_STAT_RDRF_MASK) {
        char c = (char)(SERIAL5->DATA & 0xFF);
        xQueueSendFromISR(SERIAL5_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}