#include "MIMXRT1062.h"

extern unsigned long __itcm_start__;
extern unsigned long __itcm_end__;
extern unsigned long __itcm_load__;
extern unsigned long __dtcm_start__;
extern unsigned long __dtcm_end__;
extern unsigned long __dtcm_load__;
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;
extern unsigned long __dma_buffer_start__;

extern unsigned long __flexram_bank_config__;
extern unsigned long __vectors_start__;
extern unsigned long _vStackTop;

static void memory_copy(uint32_t *dest, const uint32_t *src, uint32_t *dest_end);
static void memory_clear(uint32_t *dest, uint32_t *dest_end);
static void dma_config();

extern void Configure_PLL_CLK(void);
extern void pwm_init(void);
extern int main();

__attribute__((section(".startup")))
void Reset_Handler(void) {
    
    __disable_irq();

    IOMUXC_GPR->GPR17 = (uint32_t)&__flexram_bank_config__;
    IOMUXC_GPR->GPR16 = 0x00200007; // use FLEXRAM_BANK_CFG, ITCM and DTCM Enabled
    __set_MSP((uint32_t)&_vStackTop);

    SCB->VTOR = (uint32_t)&__vectors_start__;
    __DSB();
    __ISB();

    // Disable Watchdog
    WDOG1->WMCR &= ~(uint16_t) WDOG_WMCR_PDE_MASK;
    WDOG2->WMCR &= ~(uint16_t) WDOG_WMCR_PDE_MASK;

    WDOG1->WCR &= ~(uint16_t) WDOG_WCR_WDE_MASK;
    WDOG2->WCR &= ~(uint16_t) WDOG_WCR_WDE_MASK;

    RTWDOG->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
    RTWDOG->TOVAL = 0xFFFF;
    RTWDOG->CS = (uint32_t) ((RTWDOG->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;

    memory_copy(&__itcm_start__, &__itcm_load__, &__itcm_end__);
    memory_copy(&__dtcm_start__, &__dtcm_load__, &__dtcm_end__);
    memory_clear(&__bss_start__, &__bss_end__);

    Configure_PLL_CLK();
    dma_config();
    pwm_init();

    // enable FPU
    SCB->CPACR |= 0x00F00000;

    // Enable instruction and data caches
    if (SCB_CCR_IC_Msk != (SCB_CCR_IC_Msk & SCB->CCR)) {
        SCB_InvalidateICache();
        SCB_EnableICache();
    }
    if (SCB_CCR_DC_Msk != (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_InvalidateDCache();
        SCB_EnableDCache();
    }

    __enable_irq();
    __DSB();
    __ISB();

    main();

    while(1) {}
    
}

__attribute__((section(".startup")))
static void memory_copy(uint32_t *dest, const uint32_t *src, uint32_t *dest_end) {
    if (dest == src)
        return;
    while (dest < dest_end)
    {
        *dest++ = *src++;
    }
}

__attribute__((section(".startup")))
static void memory_clear(uint32_t *dest, uint32_t *dest_end) {
    while (dest < dest_end)
    {
        *dest++ = 0;
    }
}

__attribute__((section(".startup")))
static void dma_config() {
    ARM_MPU_Disable();

    MPU->RBAR = ARM_MPU_RBAR(6, (uint32_t)&__dma_buffer_start__);
    MPU->RASR = ARM_MPU_RASR(1, ARM_MPU_AP_FULL, 0, 1, 0, 0, 0, ARM_MPU_REGION_SIZE_4KB);

    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
}