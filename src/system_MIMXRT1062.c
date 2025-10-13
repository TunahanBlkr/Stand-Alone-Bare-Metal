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
**     Build:               b201012
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2020 NXP
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
 * @brief Device specific configuration file for MIMXRT1062 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "MIMXRT1062.h"




/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Secure mode */
  #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  SCB_NS->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Non-secure mode */
  #endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

#if defined(__MCUXPRESSO)
    extern uint32_t g_pfnVectors[];  // Vector table defined in startup code
    SCB->VTOR = (uint32_t)g_pfnVectors;
#endif

/* Disable Watchdog Power Down Counter */
    WDOG1->WMCR &= ~(uint16_t) WDOG_WMCR_PDE_MASK;
    WDOG2->WMCR &= ~(uint16_t) WDOG_WMCR_PDE_MASK;

/* Watchdog disable */

#if (DISABLE_WDOG)
    if ((WDOG1->WCR & WDOG_WCR_WDE_MASK) != 0U)
    {
        WDOG1->WCR &= ~(uint16_t) WDOG_WCR_WDE_MASK;
    }
    if ((WDOG2->WCR & WDOG_WCR_WDE_MASK) != 0U)
    {
        WDOG2->WCR &= ~(uint16_t) WDOG_WCR_WDE_MASK;
    }
    if ((RTWDOG->CS & RTWDOG_CS_CMD32EN_MASK) != 0U)
    {
        RTWDOG->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
    }
    else
    {
        RTWDOG->CNT = 0xC520U;
        RTWDOG->CNT = 0xD928U;
    }
    RTWDOG->TOVAL = 0xFFFF;
    RTWDOG->CS = (uint32_t) ((RTWDOG->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;
#endif /* (DISABLE_WDOG) */

    /* Disable Systick which might be enabled by bootrom */
    if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) != 0U)
    {
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }

/* Enable instruction and data caches */
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    if (SCB_CCR_IC_Msk != (SCB_CCR_IC_Msk & SCB->CCR)) {
        SCB_EnableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk != (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_EnableDCache();
    }
#endif

  SystemInitHook();
}

/* ----------------------------------------------------------------------------
   -- SystemInitHook()
   ---------------------------------------------------------------------------- */

__attribute__ ((weak)) void SystemInitHook (void) {
  /* Void implementation of the weak function. */
}
