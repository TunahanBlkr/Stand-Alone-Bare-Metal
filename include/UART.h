#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "Pins.h"

#define UART_FIFO_SIZE       4
#define UART_BUFFER_SIZE     128
#define UART_CLK_FREQ        CCM_UART_CLK_HZ

#define LPUART_TXCOUNT(uart)    (((uart)->WATER & LPUART_WATER_TXCOUNT_MASK) >> LPUART_WATER_TXCOUNT_SHIFT)

#define SERIAL1 LPUART6
#define SERIAL1_IRQn LPUART6_IRQn

#define SERIAL2 LPUART4
#define SERIAL2_IRQn LPUART4_IRQn

#define SERIAL3 LPUART2
#define SERIAL3_IRQn LPUART2_IRQn

#define SERIAL4 LPUART3
#define SERIAL4_IRQn LPUART3_IRQn

#define SERIAL5 LPUART8
#define SERIAL5_IRQn LPUART8_IRQn

#define SERIAL6 LPUART1
#define SERIAL6_IRQn LPUART1_IRQn

#define SERIAL7 LPUART7
#define SERIAL7_IRQn LPUART7_IRQn

#define SERIAL8 LPUART5
#define SERIAL8_IRQn LPUART5_IRQn

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

void Serial8_Init(uint32_t baudrate); // Pin 34/RX8, 35/TX8 ( Kullanma!!! )
void Serial8_Write_Char(char c);
void Serial8_Write_String(const char *str);
char Serial8_Read_Char(void);

//SERIAL8 Teensy4.1 Tarafindan Ethernet icin kullaniliyor.

#ifdef __cplusplus
}
#endif

#endif