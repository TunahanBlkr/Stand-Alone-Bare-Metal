#include <stdint.h>
#include "MIMXRT1062.h"

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
        SCB_InvalidateICache();
        SCB_EnableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk != (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_InvalidateDCache();
        SCB_EnableDCache();
    }
#endif

  SystemInitHook();
}

/* ----------------------------------------------------------------------------
   -- SystemInitHook()
   ---------------------------------------------------------------------------- */

__attribute__ ((weak)) void SystemInitHook (void) {

    // ITCM and DTCM enable
    IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_INIT_ITCM_EN(1) | IOMUXC_GPR_GPR16_INIT_DTCM_EN(1);

    // Fuse konfigurasyonunu kullan
    IOMUXC_GPR->GPR16 &= ~IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL_MASK;
    IOMUXC_GPR->GPR16 |= IOMUXC_GPR_GPR16_FLEXRAM_BANK_CFG_SEL(0);

    IOMUXC_GPR->GPR3 |= IOMUXC_GPR_GPR3_OCRAM_CTL(0x3U); // read data wait enable

    CCM_ANALOG->PLL_ARM |= CCM_ANALOG_PLL_ARM_BYPASS_MASK; // PLL bypass enable
    
    CCM_ANALOG->PLL_ARM |= CCM_ANALOG_PLL_ARM_POWERDOWN_MASK; // PLL'i Kapat

    CCM_ANALOG->PLL_ARM &= ~CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK;
    CCM_ANALOG->PLL_ARM |= CCM_ANALOG_PLL_ARM_DIV_SELECT(CCM_ANALOG_PLL_ARM_MULT_VALUE); // Carpan Degeri Ayari ( 24MHz * 22 = 528MHz )

    CCM->CACRR &= ~CCM_CACRR_ARM_PODF_MASK;
    CCM->CACRR |= CCM_CACRR_ARM_PODF(CCM_ANALOG_PLL_ARM_DIV_VALUE - 1U); // Saat Bolucu Degeri Ayari

    CCM_ANALOG->PLL_ARM &= ~CCM_ANALOG_PLL_ARM_POWERDOWN_MASK; // PLL'e Guc Ver

    CCM_ANALOG->PLL_ARM &= ~CCM_ANALOG_PLL_ARM_ENABLE_MASK; // Ne olur ne olmaz diye once biti temizle
    CCM_ANALOG->PLL_ARM |= CCM_ANALOG_PLL_ARM_ENABLE_MASK; // PLL'i Aktive Et

    //PLL Kilitlenene Kadar Bekle
    volatile uint32_t timeout1 = 0x00FFFFFFU;
    while (!(CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_MASK))
    {
        if(--timeout1 == 0) break;
    }

    if (CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_MASK) {
        CCM_ANALOG->PLL_ARM &= ~CCM_ANALOG_PLL_ARM_BYPASS_MASK; // PLL bypass disable
    } else {
        // PLL Lock Fail
    }

    CCM->CBCMR &= ~CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK;
    CCM->CBCMR |=  CCM_CBCMR_PRE_PERIPH_CLK_SEL(0);  // 0 = PLL2 (528 MHz)
    (void)CCM->CBCMR;

    CCM->CBCDR &= ~CCM_CBCDR_PERIPH_CLK_SEL_MASK;
    CCM->CBCDR |= CCM_CBCDR_PERIPH_CLK_SEL(0); // PLL2 secildi
    while(CCM->CDHIPR & CCM_CDHIPR_PERIPH_CLK_SEL_BUSY_MASK);

    // AHB clock = PLL2 / 4 (132 MHz)
    CCM->CBCDR &= ~CCM_CBCDR_AHB_PODF_MASK;
    CCM->CBCDR |= CCM_CBCDR_AHB_PODF(AHB_DIV_VALUE - 1U); // bolucu = 4
    while(CCM->CDHIPR & CCM_CDHIPR_AHB_PODF_BUSY_MASK);

    // IPG clock = AHB / 2 (66 MHz)
    CCM->CBCDR &= ~CCM_CBCDR_IPG_PODF_MASK;
    CCM->CBCDR |= CCM_CBCDR_IPG_PODF(IPG_DIV_VALUE - 1U); // bolucu = 2
    while(CCM->CDHIPR & CCM_CDHIPR_AHB_PODF_BUSY_MASK); // IPG_PODF degisimi AHB handshake ile yapilir; ayri IPG_BUSY biti yok

    __DSB();
    __ISB();


    // Bazi projeler DWT_CYCCNT sifirlayip core frekansini olcer veya GPIO toggle ile test eder.
    // Asagida bunun icin ornek bir test fonksiyonu cagirabilirsin.
    
/*
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    // ... biraz bekle ...
    uint32_t cycles = DWT->CYCCNT;
*/
}