/*
** ###################################################################
**     Processors:          MIMXRT1062CVJ5A
**                          MIMXRT1062CVL5A
**                          MIMXRT1062DVJ6A
**                          MIMXRT1062DVL6A
**
**     Compilers:           Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    IMXRT1060RM Rev.1, 12/2018 | IMXRT1060SRM Rev.3
**     Version:             rev. 1.2, 2019-04-29
**     Build:               b191113
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2019 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2017-01-10)
**         Initial version.
**     - rev. 1.0 (2018-11-16)
**         Update header files to align with IMXRT1060RM Rev.0.
**     - rev. 1.1 (2018-11-27)
**         Update header files to align with IMXRT1060RM Rev.1.
**     - rev. 1.2 (2019-04-29)
**         Add SET/CLR/TOG register group to register CTRL, STAT, CHANNELCTRL, CH0STAT, CH0OPTS, CH1STAT, CH1OPTS, CH2STAT, CH2OPTS, CH3STAT, CH3OPTS of DCP module.
**
** ###################################################################
*/

/*!
 * @file MIMXRT1062
 * @version 1.2
 * @date 2019-04-29
 * @brief Device specific configuration file for MIMXRT1062 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef _SYSTEM_MIMXRT1062_H_
#define _SYSTEM_MIMXRT1062_H_                    /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#ifndef DISABLE_WDOG
  #define DISABLE_WDOG                 1
#endif

/* Define clock source values */

#define CPU_XTAL_CLK_HZ                   24000000UL          /* Value of the external crystal or oscillator clock frequency in Hz */

#define CCM_ANALOG_PLL_ARM_CLK_HZ         528000000UL
#define CCM_ANALOG_PLL_SYS_CLK_HZ         528000000UL
#define CCM_ANALOG_PLL_USB1_CLK_HZ        480000000UL

#define CCM_AHB_CLK_HZ                    132000000UL
#define CCM_IPG_CLK_HZ                    44000000UL
#define CCM_PERCLK_CLK_HZ                 44000000UL
#define CCM_USDHC1_CLK_HZ                 66000000UL
#define CCM_USDHC2_CLK_HZ                 66000000UL
#define CCM_CSI_CLK_HZ                    60000000UL
#define CCM_LPSPI_CLK_HZ                  66000000UL
#define CCM_TRACE_CLK_HZ                  132000000UL
#define CCM_LPI2C_CLK_HZ                  60000000UL
#define CCM_CAN_CLK_HZ                    40000000UL
#define CCM_UART_CLK_HZ                   24000000UL
#define CCM_LCDIF_CLK_HZ                  // PLL5 AYARLA - PLL5 / 4

#ifdef __cplusplus
}
#endif

#endif  /* _SYSTEM_MIMXRT1062_H_ */