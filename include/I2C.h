#ifndef I2C_H
#define I2C_H

#ifdef _cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "Pins.h"
#include "stdint.h"

#define I2C_BUFFER_SIZE 64

#define Wire1       LPI2C1
#define Wire1_IRQn  LPI2C1_IRQn

#define Wire2       LPI2C3
#define Wire2_IRQn  LPI2C3_IRQn

#define Wire3       LPI2C4
#define Wire3_IRQn  LPI2C4_IRQn

typedef enum {
    FAST,
    FAST_PLUS,
    HIGH_SPEED
} SPEED_MODE;

void Wire1_Init(SPEED_MODE mode);
void Wire1_Transmit(uint16_t* commands, uint8_t command_count);
uint8_t Wire1_Receive(void);

void Wire2_Init(SPEED_MODE mode);
void Wire2_Transmit(uint16_t* commands, uint8_t command_count);
uint8_t Wire2_Receive(void);

void Wire3_Init(SPEED_MODE mode);
void Wire3_Transmit(uint16_t* commands, uint8_t command_count);
uint8_t Wire3_Receive(void);

#ifdef _cplusplus
}
#endif

#endif