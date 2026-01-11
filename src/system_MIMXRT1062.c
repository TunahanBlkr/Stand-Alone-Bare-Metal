#include "MIMXRT1062.h"

extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

extern unsigned long _vStackTop;
extern unsigned long __vectors_start__;

static void data_init(unsigned int romstart, unsigned int start, unsigned int len);
static void bss_init(unsigned int start, unsigned int len);

extern void Configure_PLL_CLK(void);
extern int main();

__attribute__((section(".startup")))
void Reset_Handler(void) {

    //FlexRAM Config
    /*
    IOMUXC_GPR->GPR17 = 0xAAAAAAAA;
    IOMUXC_GPR->GPR16 = 0x00200007; // use FLEXRAM_BANK_CFG, DTCM Enabled
    IOMUXC_GPR->GPR14 = 0x00AA0000; // 512KB DTCM Size
    */
    __disable_irq();
    __asm__ volatile("MSR MSP, %0" : : "r" (&_vStackTop) : );

    SCB->VTOR = (uint32_t)&__vectors_start__;

    /*
    asm volatile (
        "ldr r1, =__vectors\n"
        "ldr r0, =0xE000ED08\n"
        "str r1, [r0]\n"
        :
        :
        : "r0", "r1", "memory"
    );
    */

    __DSB();
    __ISB();

    /* Disable Watchdog Power Down Counter */
    WDOG1->WMCR &= ~(uint16_t) WDOG_WMCR_PDE_MASK;
    WDOG2->WMCR &= ~(uint16_t) WDOG_WMCR_PDE_MASK;

    /* Watchdog disable */
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

    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *SectionTableAddr;

    // Load base address of Global Section Table
    SectionTableAddr = &__data_section_table;

    // Copy the data sections from flash to SRAM.
    while (SectionTableAddr < &__data_section_table_end) {
        LoadAddr = *SectionTableAddr++;
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        data_init(LoadAddr, ExeAddr, SectionLen);
    }

    // At this point, SectionTableAddr = &__bss_section_table;
    // Zero fill the bss segment
    while (SectionTableAddr < &__bss_section_table_end) {
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        bss_init(ExeAddr, SectionLen);
    }

    Configure_PLL_CLK();

    // enable FPU
#if defined(__FPU_PRESENT) && __FPU_PRESENT
    __asm__ volatile (
    "LDR r0, =0xE000ED88\n"
    "LDR r1, [R0]\n"             // ; Read CPACR
    "ORR r1, R1, (0xF << 20)\n"  // ; Set bits 20-23 to enable CP10 and CP11 coprocessors
    "STR r1, [R0]"               // ; Write back the modified value to the CPACRDSBISB) ;
    );
#endif

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

    __enable_irq();

    __DSB();
    __ISB();

    /**/
    /**/


    main();

    while(1) {}
    
}

__attribute__((section(".startup")))
static void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int *pulSrc = (unsigned int*) romstart;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = *pulSrc++;
}

__attribute__((section(".startup")))
static void bss_init(unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = 0;
}