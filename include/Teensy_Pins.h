#ifndef TEENSY_PINS_H
#define TEENSY_PINS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MIMXRT1062.h"

/********************************************************************************************************************************/
/*                                                        PIN_MASK                                                              */
/********************************************************************************************************************************/

#define PIN0_BIT        3
#define PIN1_BIT        2
#define PIN2_BIT        4
#define PIN3_BIT        5
#define PIN4_BIT        6
#define PIN5_BIT        8
#define PIN6_BIT        10
#define PIN7_BIT        17
#define PIN8_BIT        16
#define PIN9_BIT        11
#define PIN10_BIT       0
#define PIN11_BIT       2
#define PIN12_BIT       1
#define PIN13_BIT       3
#define PIN14_BIT       18
#define PIN15_BIT       19
#define PIN16_BIT       23
#define PIN17_BIT       22
#define PIN18_BIT       17
#define PIN19_BIT       16
#define PIN20_BIT       26
#define PIN21_BIT       27
#define PIN22_BIT       24
#define PIN23_BIT       25
#define PIN24_BIT       12
#define PIN25_BIT       13
#define PIN26_BIT       30
#define PIN27_BIT       31
#define PIN28_BIT       18
#define PIN29_BIT       31
#define PIN30_BIT       23
#define PIN31_BIT       22
#define PIN32_BIT       12
#define PIN33_BIT       7
#define PIN34_BIT       29
#define PIN35_BIT       28
#define PIN36_BIT       18
#define PIN37_BIT       19
#define PIN38_BIT       28
#define PIN39_BIT       29
#define PIN40_BIT       20
#define PIN41_BIT       21
#define PIN42_BIT       15
#define PIN43_BIT       14
#define PIN44_BIT       13
#define PIN45_BIT       12
#define PIN46_BIT       17
#define PIN47_BIT       16
#define PIN48_BIT       24
#define PIN49_BIT       27
#define PIN50_BIT       28
#define PIN51_BIT       22
#define PIN52_BIT       26
#define PIN53_BIT       25
#define PIN54_BIT       29

#define PIN0_BITMASK	(( 1 << (PIN0_BIT) ) & 0xFFFFFFFFU)
#define PIN1_BITMASK	(( 1 << (PIN1_BIT) ) & 0xFFFFFFFFU)
#define PIN2_BITMASK	(( 1 << (PIN2_BIT) ) & 0xFFFFFFFFU)
#define PIN3_BITMASK	(( 1 << (PIN3_BIT) ) & 0xFFFFFFFFU)
#define PIN4_BITMASK	(( 1 << (PIN4_BIT) ) & 0xFFFFFFFFU)
#define PIN5_BITMASK	(( 1 << (PIN5_BIT) ) & 0xFFFFFFFFU)
#define PIN6_BITMASK	(( 1 << (PIN6_BIT) ) & 0xFFFFFFFFU)
#define PIN7_BITMASK	(( 1 << (PIN7_BIT) ) & 0xFFFFFFFFU)
#define PIN8_BITMASK	(( 1 << (PIN8_BIT) ) & 0xFFFFFFFFU)
#define PIN9_BITMASK	(( 1 << (PIN9_BIT) ) & 0xFFFFFFFFU)
#define PIN10_BITMASK	(( 1 << (PIN10_BIT) ) & 0xFFFFFFFFU)
#define PIN11_BITMASK	(( 1 << (PIN11_BIT) ) & 0xFFFFFFFFU)
#define PIN12_BITMASK	(( 1 << (PIN12_BIT) ) & 0xFFFFFFFFU)
#define PIN13_BITMASK	(( 1 << (PIN13_BIT) ) & 0xFFFFFFFFU)
#define PIN14_BITMASK	(( 1 << (PIN14_BIT) ) & 0xFFFFFFFFU)
#define PIN15_BITMASK	(( 1 << (PIN15_BIT) ) & 0xFFFFFFFFU)
#define PIN16_BITMASK	(( 1 << (PIN16_BIT) ) & 0xFFFFFFFFU)
#define PIN17_BITMASK	(( 1 << (PIN17_BIT) ) & 0xFFFFFFFFU)
#define PIN18_BITMASK	(( 1 << (PIN18_BIT) ) & 0xFFFFFFFFU)
#define PIN19_BITMASK	(( 1 << (PIN19_BIT) ) & 0xFFFFFFFFU)
#define PIN20_BITMASK	(( 1 << (PIN20_BIT) ) & 0xFFFFFFFFU)
#define PIN21_BITMASK	(( 1 << (PIN21_BIT) ) & 0xFFFFFFFFU)
#define PIN22_BITMASK	(( 1 << (PIN22_BIT) ) & 0xFFFFFFFFU)
#define PIN23_BITMASK	(( 1 << (PIN23_BIT) ) & 0xFFFFFFFFU)
#define PIN24_BITMASK	(( 1 << (PIN24_BIT) ) & 0xFFFFFFFFU)
#define PIN25_BITMASK	(( 1 << (PIN25_BIT) ) & 0xFFFFFFFFU)
#define PIN26_BITMASK	(( 1 << (PIN26_BIT) ) & 0xFFFFFFFFU)
#define PIN27_BITMASK	(( 1 << (PIN27_BIT) ) & 0xFFFFFFFFU)
#define PIN28_BITMASK	(( 1 << (PIN28_BIT) ) & 0xFFFFFFFFU)
#define PIN29_BITMASK	(( 1 << (PIN29_BIT) ) & 0xFFFFFFFFU)
#define PIN30_BITMASK	(( 1 << (PIN30_BIT) ) & 0xFFFFFFFFU)
#define PIN31_BITMASK	(( 1 << (PIN31_BIT) ) & 0xFFFFFFFFU)
#define PIN32_BITMASK	(( 1 << (PIN32_BIT) ) & 0xFFFFFFFFU)
#define PIN33_BITMASK	(( 1 << (PIN33_BIT) ) & 0xFFFFFFFFU)
#define PIN34_BITMASK	(( 1 << (PIN34_BIT) ) & 0xFFFFFFFFU)
#define PIN35_BITMASK	(( 1 << (PIN35_BIT) ) & 0xFFFFFFFFU)
#define PIN36_BITMASK	(( 1 << (PIN36_BIT) ) & 0xFFFFFFFFU)
#define PIN37_BITMASK	(( 1 << (PIN37_BIT) ) & 0xFFFFFFFFU)
#define PIN38_BITMASK	(( 1 << (PIN38_BIT) ) & 0xFFFFFFFFU)
#define PIN39_BITMASK	(( 1 << (PIN39_BIT) ) & 0xFFFFFFFFU)
#define PIN40_BITMASK	(( 1 << (PIN40_BIT) ) & 0xFFFFFFFFU)
#define PIN41_BITMASK	(( 1 << (PIN41_BIT) ) & 0xFFFFFFFFU)
#define PIN42_BITMASK	(( 1 << (PIN42_BIT) ) & 0xFFFFFFFFU)
#define PIN43_BITMASK	(( 1 << (PIN43_BIT) ) & 0xFFFFFFFFU)
#define PIN44_BITMASK	(( 1 << (PIN44_BIT) ) & 0xFFFFFFFFU)
#define PIN45_BITMASK	(( 1 << (PIN45_BIT) ) & 0xFFFFFFFFU)
#define PIN46_BITMASK	(( 1 << (PIN46_BIT) ) & 0xFFFFFFFFU)
#define PIN47_BITMASK	(( 1 << (PIN47_BIT) ) & 0xFFFFFFFFU)
#define PIN48_BITMASK	(( 1 << (PIN48_BIT) ) & 0xFFFFFFFFU)
#define PIN49_BITMASK	(( 1 << (PIN49_BIT) ) & 0xFFFFFFFFU)
#define PIN50_BITMASK	(( 1 << (PIN50_BIT) ) & 0xFFFFFFFFU)
#define PIN51_BITMASK	(( 1 << (PIN51_BIT) ) & 0xFFFFFFFFU)
#define PIN52_BITMASK	(( 1 << (PIN52_BIT) ) & 0xFFFFFFFFU)
#define PIN53_BITMASK	(( 1 << (PIN53_BIT) ) & 0xFFFFFFFFU)
#define PIN54_BITMASK	(( 1 << (PIN54_BIT) ) & 0xFFFFFFFFU)

/********************************************************************************************************************************/
/*                                                        PIN_GPIO                                                              */
/********************************************************************************************************************************/

#define PIN0_GPIO       GPIO1
#define PIN1_GPIO       GPIO1
#define PIN2_GPIO       GPIO4
#define PIN3_GPIO       GPIO4
#define PIN4_GPIO       GPIO4
#define PIN5_GPIO       GPIO4
#define PIN6_GPIO       GPIO2
#define PIN7_GPIO       GPIO2
#define PIN8_GPIO       GPIO2
#define PIN9_GPIO       GPIO2
#define PIN10_GPIO      GPIO2
#define PIN11_GPIO      GPIO2
#define PIN12_GPIO      GPIO2
#define PIN13_GPIO      GPIO2
#define PIN14_GPIO      GPIO1
#define PIN15_GPIO      GPIO1
#define PIN16_GPIO      GPIO1
#define PIN17_GPIO      GPIO1
#define PIN18_GPIO      GPIO1
#define PIN19_GPIO      GPIO1
#define PIN20_GPIO      GPIO1
#define PIN21_GPIO      GPIO1
#define PIN22_GPIO      GPIO1
#define PIN23_GPIO      GPIO1
#define PIN24_GPIO      GPIO1
#define PIN25_GPIO      GPIO1
#define PIN26_GPIO      GPIO1
#define PIN27_GPIO      GPIO1
#define PIN28_GPIO      GPIO3
#define PIN29_GPIO      GPIO4
#define PIN30_GPIO      GPIO3
#define PIN31_GPIO      GPIO3
#define PIN32_GPIO      GPIO2
#define PIN33_GPIO      GPIO4
#define PIN34_GPIO      GPIO2
#define PIN35_GPIO      GPIO2
#define PIN36_GPIO      GPIO2
#define PIN37_GPIO      GPIO2
#define PIN38_GPIO      GPIO1
#define PIN39_GPIO      GPIO1
#define PIN40_GPIO      GPIO1
#define PIN41_GPIO      GPIO1
#define PIN42_GPIO      GPIO3
#define PIN43_GPIO      GPIO3
#define PIN44_GPIO      GPIO3
#define PIN45_GPIO      GPIO3
#define PIN46_GPIO      GPIO3
#define PIN47_GPIO      GPIO3
#define PIN48_GPIO      GPIO4
#define PIN49_GPIO      GPIO4
#define PIN50_GPIO      GPIO4
#define PIN51_GPIO      GPIO4
#define PIN52_GPIO      GPIO4
#define PIN53_GPIO      GPIO4
#define PIN54_GPIO      GPIO4

/********************************************************************************************************************************/
/*                                                        PIN_MUX                                                               */
/********************************************************************************************************************************/

#define PIN0_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_03
#define PIN1_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_02
#define PIN2_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_04
#define PIN3_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_05
#define PIN4_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06
#define PIN5_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_08
#define PIN6_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_10
#define PIN7_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_01
#define PIN8_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_00
#define PIN9_MUX        kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_11
#define PIN10_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_00
#define PIN11_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_02
#define PIN12_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_01
#define PIN13_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03
#define PIN14_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_02
#define PIN15_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_03
#define PIN16_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_07
#define PIN17_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_06
#define PIN18_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_01
#define PIN19_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_00
#define PIN20_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_10
#define PIN21_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_11
#define PIN22_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_08
#define PIN23_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_09
#define PIN24_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_12
#define PIN25_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_13
#define PIN26_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_14
#define PIN27_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_15
#define PIN28_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32
#define PIN29_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_31
#define PIN30_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_37
#define PIN31_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_36
#define PIN32_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B0_12
#define PIN33_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_07
#define PIN34_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_13
#define PIN35_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_12
#define PIN36_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02
#define PIN37_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_03
#define PIN38_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_12
#define PIN39_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_13
#define PIN40_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_04
#define PIN41_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_05
#define PIN42_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_03
#define PIN43_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_02
#define PIN44_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_01
#define PIN45_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_00
#define PIN46_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_05
#define PIN47_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B0_04
#define PIN48_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_24
#define PIN49_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_27
#define PIN50_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_28
#define PIN51_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_22
#define PIN52_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_26
#define PIN53_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_25
#define PIN54_MUX       kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_29

/********************************************************************************************************************************/
/*                                                        PIN_PAD                                                               */
/********************************************************************************************************************************/

#define PIN0_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_03
#define PIN1_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_02
#define PIN2_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_04
#define PIN3_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_05
#define PIN4_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_06
#define PIN5_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_08
#define PIN6_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_10
#define PIN7_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_01
#define PIN8_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_00
#define PIN9_PAD        kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_11
#define PIN10_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_00
#define PIN11_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_02
#define PIN12_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_01
#define PIN13_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_03
#define PIN14_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_02
#define PIN15_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_03
#define PIN16_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_07
#define PIN17_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_06
#define PIN18_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_01
#define PIN19_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_00
#define PIN20_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_10
#define PIN21_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_11
#define PIN22_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_08
#define PIN23_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_09
#define PIN24_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_12
#define PIN25_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_13
#define PIN26_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_14
#define PIN27_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_15
#define PIN28_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_32
#define PIN29_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_31
#define PIN30_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_37
#define PIN31_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_36
#define PIN32_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B0_12
#define PIN33_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_07
#define PIN34_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_13
#define PIN35_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_12
#define PIN36_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_02
#define PIN37_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_B1_03
#define PIN38_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_12
#define PIN39_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_13
#define PIN40_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_04
#define PIN41_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B1_05
#define PIN42_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B0_03
#define PIN43_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B0_02
#define PIN44_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B0_01
#define PIN45_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B0_00
#define PIN46_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B0_05
#define PIN47_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_SD_B0_04
#define PIN48_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_24
#define PIN49_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_27
#define PIN50_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_28
#define PIN51_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_22
#define PIN52_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_26
#define PIN53_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_25
#define PIN54_PAD       kIOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_29

// Belki lazim olursa pinMode() fonksiyonu yazilabilir.
// void pinMode(GPIO_Type* PIN_GPIO, uint8_t PIN_MUX, uint8_t PIN_PAD, uint8_t PIN_MASK, uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif