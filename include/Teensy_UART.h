#ifndef TEENSY_UART_H
#define TEENSY_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "FreeRTOS.h"
#include "queue.h"

#define UART_TX_BUFFER_SIZE     128
#define UART_RX_BUFFER_SIZE     128
#define UART_CLK_FREQ           CCM_UART_CLK_HZ

#define SERIAL1 LPUART6
#define SERIAL2 LPUART4
#define SERIAL3 LPUART2
#define SERIAL4 LPUART3
#define SERIAL5 LPUART8
#define SERIAL6 LPUART1
#define SERIAL7 LPUART7
#define SERIAL8 LPUART5

#define LPUART_TXCOUNT(uart)    (((uart)->WATER & LPUART_WATER_TXCOUNT_MASK) >> LPUART_WATER_TXCOUNT_SHIFT)

extern QueueHandle_t SERIAL1_TX_QUEUE;
extern QueueHandle_t SERIAL1_RX_QUEUE;

extern QueueHandle_t SERIAL2_TX_QUEUE;
extern QueueHandle_t SERIAL2_RX_QUEUE;

extern QueueHandle_t SERIAL3_TX_QUEUE;
extern QueueHandle_t SERIAL3_RX_QUEUE;

extern QueueHandle_t SERIAL4_TX_QUEUE;
extern QueueHandle_t SERIAL4_RX_QUEUE;

extern QueueHandle_t SERIAL5_TX_QUEUE;
extern QueueHandle_t SERIAL5_RX_QUEUE;

extern QueueHandle_t SERIAL6_TX_QUEUE;
extern QueueHandle_t SERIAL6_RX_QUEUE;

extern QueueHandle_t SERIAL7_TX_QUEUE;
extern QueueHandle_t SERIAL7_RX_QUEUE;

extern QueueHandle_t SERIAL8_TX_QUEUE;
extern QueueHandle_t SERIAL8_RX_QUEUE;

extern uint8_t LPUART_FIFO_SIZE;

void Serial1_Init(uint32_t baudrate); // Pin 0/RX1, 1/TX1
void Serial1_Write_Char(char c);
void Serial1_Write_String(const char *str);
char Serial1_Read_Char(void);

void Serial2_Init(uint32_t baudrate); // Pin 7/RX2, 8/TX2
void Serial2_Write_Char(char c);
void Serial2_Write_String(const char *str);
char Serial2_Read_Char(void);

void Serial3_Init(uint32_t baudrate); // Pin 15/RX3, 14/TX3
void Serial3_Write_Char(char c);
void Serial3_Write_String(const char *str);
char Serial3_Read_Char(void);

void Serial4_Init(uint32_t baudrate); // Pin 16/RX4, 17/TX4
void Serial4_Write_Char(char c);
void Serial4_Write_String(const char *str);
char Serial4_Read_Char(void);

void Serial5_Init(uint32_t baudrate); // Pin 21/RX5, 20/TX5
void Serial5_Write_Char(char c);
void Serial5_Write_String(const char *str);
char Serial5_Read_Char(void);

void Serial6_Init(uint32_t baudrate); // Pin 25/RX6, 24/TX6
void Serial6_Write_Char(char c);
void Serial6_Write_String(const char *str);
char Serial6_Read_Char(void);

void Serial7_Init(uint32_t baudrate); // Pin 28/RX7, 29/TX7
void Serial7_Write_Char(char c);
void Serial7_Write_String(const char *str);
char Serial7_Read_Char(void);

void Serial8_Init(uint32_t baudrate); // Pin 34/RX7, 35/TX7 ( Kullanma!!! )
void Serial8_Write_Char(char c);
void Serial8_Write_String(const char *str);
char Serial8_Read_Char(void);

void LPUART1_IRQHandler(void);
void LPUART2_IRQHandler(void);
void LPUART3_IRQHandler(void);
void LPUART4_IRQHandler(void);
void LPUART5_IRQHandler(void);
void LPUART6_IRQHandler(void);
void LPUART7_IRQHandler(void);
void LPUART8_IRQHandler(void);

//LUPART8 Teensy4.1 Tarafindan Ethernet icin kullaniliyor.

#ifdef __cplusplus
}
#endif

#endif