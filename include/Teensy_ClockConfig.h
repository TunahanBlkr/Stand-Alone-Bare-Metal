#ifndef TEENSY_CLOCKCONFIG_H
#define TEENSY_CLOCKCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"

void Configure_PLL_CLK(void);
void OpenCloks(void);

void PLL_ARM_CLK_Init(void);
void PLL_SYS_CLK_Init(void);
void PLL_USB1_CLK_Init(void);
void PLL_VIDEO_CLK_Init(void);

void USDHC1_CLK_Init(void);
void USDHC2_CLK_Init(void);
void CSI_CLK_Init(void);
void LPSPI_CLK_Init(void);
void TRACE_CLK_Init(void);
void LPI2C_CLK_Init(void);
void CAN_CLK_Init(void);
void UART_CLK_Init(void);
void LCDIF_CLK_Init(void);

#ifdef __cplusplus
}
#endif

#endif