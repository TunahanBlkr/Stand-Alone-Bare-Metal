#include "I2C.h"

QueueHandle_t Wire1_TX_QUEUE;
QueueHandle_t Wire1_RX_QUEUE;

QueueHandle_t Wire2_TX_QUEUE;
QueueHandle_t Wire2_RX_QUEUE;

QueueHandle_t Wire3_TX_QUEUE;
QueueHandle_t Wire3_RX_QUEUE;

static void I2C_Common_Init(LPI2C_Type* LPI2C, SPEED_MODE mode) {
    LPI2C->MCR |=  LPI2C_MCR_RST_MASK; // Software Reset
    LPI2C->MCR &= ~LPI2C_MCR_RST_MASK; // Clear Reset Bit

    LPI2C->MCR &=  ~LPI2C_MCR_MEN_MASK; // Master Logic Disable

    LPI2C->MCR |=  LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK; // Clear Receive and Transmit FIFO

    LPI2C->MIER |= LPI2C_MIER_RDIE_MASK; //Receive Data Interrupt Enable

    LPI2C->MCFGR1 = LPI2C_MCFGR1_PRESCALE(1); // Prescale Configuration

    if(mode == FAST) {
        LPI2C->MCFGR2 = LPI2C_MCFGR2_FILTSDA(2) | LPI2C_MCFGR2_FILTSCL(2);
        LPI2C->MCCR0  = LPI2C_MCCR0_DATAVD(8) | LPI2C_MCCR0_SETHOLD(17) | LPI2C_MCCR0_CLKHI(31) | LPI2C_MCCR0_CLKLO(40); // 400KHz
    } else {
        LPI2C->MCFGR2 = LPI2C_MCFGR2_FILTSDA(0) | LPI2C_MCFGR2_FILTSCL(0);
        LPI2C->MCCR0  = LPI2C_MCCR0_DATAVD(3) | LPI2C_MCCR0_SETHOLD(9) | LPI2C_MCCR0_CLKHI(11) | LPI2C_MCCR0_CLKLO(16); // 1MHz
        if(mode == HIGH_SPEED) {
            LPI2C->MCCR1 = LPI2C_MCCR1_DATAVD(1) | LPI2C_MCCR1_SETHOLD(4) | LPI2C_MCCR1_CLKHI(2) | LPI2C_MCCR1_CLKLO(4); // 3.3MHz
        }
    }

    LPI2C->MFCR = LPI2C_MFCR_RXWATER(0) | LPI2C_MFCR_TXWATER(2); // Receive/Transmit Watermark

    // Master Logic Enable Islemi Baska Bir Yerde Yapilacak.
}

/********************************************************************************************************************************/
/*                                                         WIRE1                                                                */
/********************************************************************************************************************************/

void Wire1_Init(SPEED_MODE mode) {
    CCM->CCGR2 |= CCM_CCGR2_CG3_MASK;

    I2C_Common_Init(Wire1, mode);

    Wire1_TX_QUEUE = xQueueCreate(I2C_BUFFER_SIZE, sizeof(uint16_t));
    Wire1_RX_QUEUE = xQueueCreate(I2C_BUFFER_SIZE, sizeof(uint8_t));

    NVIC_SetPriority(Wire1_IRQn, 4);
    NVIC_EnableIRQ(Wire1_IRQn);

    // Wire1 = LPI2C1
    // LPI2C1_SCL = PIN19, LPI2C1_SDA = PIN18
    IOMUXC->SW_MUX_CTL_PAD[PIN19_MUX] = 0x3 | 0x10; // ALT3 = LPI2C1_SCL
    IOMUXC->SW_MUX_CTL_PAD[PIN18_MUX] = 0x3 | 0x10; // ALT3 = LPI2C1_SDA

    if(mode == FAST) {
        // SRE = Disable, DSE = R0/6, SPEED = fast(150MHz), ODE = Enable, PKE = Enable, PUE= Pull, PUS = 22K Ohm Pull up, HYS = Enable
        IOMUXC->SW_PAD_CTL_PAD[PIN19_PAD] = 0x1F8B0;
        IOMUXC->SW_PAD_CTL_PAD[PIN18_PAD] = 0x1F8B0;
    }else {
        // SRE = Enable, DSE = R0/6, SPEED = max(200MHz), ODE = Enable, PKE = Enable, PUE= Pull, PUS = 22K Ohm Pull up, HYS = Enable
        // DSE = R0/7 yapmak icin 0x1F8F9
        IOMUXC->SW_PAD_CTL_PAD[PIN19_PAD] = 0x1F8F1;
        IOMUXC->SW_PAD_CTL_PAD[PIN18_PAD] = 0x1F8F1;
    }

    Wire1->MCR |=  LPI2C_MCR_MEN_MASK; // Master Logic Enable
}

void Wire1_Transmit(uint16_t* commands, uint8_t command_count) {
    for(int i = 0; i < command_count; i++) {
        xQueueSend(Wire1_TX_QUEUE, (commands + i), portMAX_DELAY);
    }

    Wire1->MIER |= LPI2C_MIER_TDIE_MASK; //Transmit Data Interrupt Enable
}

uint8_t Wire1_Receive(void) {
    uint8_t c;
    xQueueReceive(Wire1_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

void LPI2C1_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    //TX interrupt
    if(Wire1->MSR & LPI2C_MSR_TDF_MASK) {
        uint8_t space = 4 - (Wire1->MFSR & LPI2C_MFSR_TXCOUNT_MASK);
        uint16_t c;
        while(space--) {
            if(xQueueReceiveFromISR(Wire1_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                Wire1->MTDR = c;
            } else {
                Wire1->MIER &= ~LPI2C_MIER_TDIE_MASK;
                break;
            }
        }
    }

    if(Wire1->MSR & LPI2C_MSR_RDF_MASK) {
        while(!(Wire1->MRDR & LPI2C_MRDR_RXEMPTY_MASK)) {
            uint8_t c = (uint8_t)(Wire1->MRDR & 0xFF);
            xQueueSendFromISR(Wire1_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/********************************************************************************************************************************/
/*                                                         WIRE2                                                                */
/********************************************************************************************************************************/

void Wire2_Init(SPEED_MODE mode) {
    CCM->CCGR2 |= CCM_CCGR2_CG5_MASK;

    I2C_Common_Init(Wire2, mode);

    Wire2_TX_QUEUE = xQueueCreate(I2C_BUFFER_SIZE, sizeof(uint16_t));
    Wire2_RX_QUEUE = xQueueCreate(I2C_BUFFER_SIZE, sizeof(uint8_t));

    NVIC_SetPriority(Wire2_IRQn, 4);
    NVIC_EnableIRQ(Wire2_IRQn);

    // Wire2 = LPI2C3
    // LPI2C3_SCL = PIN16, LPI2C3_SDA = PIN17
    IOMUXC->SW_MUX_CTL_PAD[PIN16_MUX] = 0x1 | 0x10; // ALT1 = LPI2C3_SCL
    IOMUXC->SW_MUX_CTL_PAD[PIN17_MUX] = 0x1 | 0x10; // ALT1 = LPI2C3_SDA

    if(mode == FAST) {
        // SRE = Disable, DSE = R0/6, SPEED = fast(150MHz), ODE = Enable, PKE = Enable, PUE= Pull, PUS = 22K Ohm Pull up, HYS = Enable
        IOMUXC->SW_PAD_CTL_PAD[PIN16_PAD] = 0x1F8B0;
        IOMUXC->SW_PAD_CTL_PAD[PIN17_PAD] = 0x1F8B0;
    }else {
        // SRE = Enable, DSE = R0/6, SPEED = max(200MHz), ODE = Enable, PKE = Enable, PUE= Pull, PUS = 22K Ohm Pull up, HYS = Enable
        // DSE = R0/7 yapmak icin 0x1F8F9
        IOMUXC->SW_PAD_CTL_PAD[PIN16_PAD] = 0x1F8F1;
        IOMUXC->SW_PAD_CTL_PAD[PIN17_PAD] = 0x1F8F1;
    }

    Wire2->MCR |=  LPI2C_MCR_MEN_MASK; // Master Logic Enable
}

void Wire2_Transmit(uint16_t* commands, uint8_t command_count) {
    for(int i = 0; i < command_count; i++) {
        xQueueSend(Wire2_TX_QUEUE, (commands + i), portMAX_DELAY);
    }

    Wire2->MIER |= LPI2C_MIER_TDIE_MASK; //Transmit Data Interrupt Enable
}

uint8_t Wire2_Receive(void) {
    uint8_t c;
    xQueueReceive(Wire2_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

void LPI2C3_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    //TX interrupt
    if(Wire2->MSR & LPI2C_MSR_TDF_MASK) {
        uint8_t space = 4 - (Wire2->MFSR & LPI2C_MFSR_TXCOUNT_MASK);
        uint16_t c;
        while(space--) {
            if(xQueueReceiveFromISR(Wire2_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                Wire2->MTDR = c;
            } else {
                Wire2->MIER &= ~LPI2C_MIER_TDIE_MASK;
                break;
            }
        }
    }

    if(Wire2->MSR & LPI2C_MSR_RDF_MASK) {
        while(!(Wire2->MRDR & LPI2C_MRDR_RXEMPTY_MASK)) {
            uint8_t c = (uint8_t)(Wire2->MRDR & 0xFF);
            xQueueSendFromISR(Wire2_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/********************************************************************************************************************************/
/*                                                         WIRE3                                                                */
/********************************************************************************************************************************/

void Wire3_Init(SPEED_MODE mode) {
    CCM->CCGR6 |= CCM_CCGR6_CG12_MASK;

    I2C_Common_Init(Wire3, mode);

    Wire3_TX_QUEUE = xQueueCreate(I2C_BUFFER_SIZE, sizeof(uint16_t));
    Wire3_RX_QUEUE = xQueueCreate(I2C_BUFFER_SIZE, sizeof(uint8_t));

    NVIC_SetPriority(Wire3_IRQn, 4);
    NVIC_EnableIRQ(Wire3_IRQn);

    // Wire3 = LPI2C4
    // LPI2C4_SCL = PIN24, LPI2C4_SDA = PIN25
    IOMUXC->SW_MUX_CTL_PAD[PIN24_MUX] = 0x0 | 0x10; // ALT0 = LPI2C4_SCL
    IOMUXC->SW_MUX_CTL_PAD[PIN25_MUX] = 0x0 | 0x10; // ALT0 = LPI2C4_SDA

    if(mode == FAST) {
        // SRE = Disable, DSE = R0/6, SPEED = fast(150MHz), ODE = Enable, PKE = Enable, PUE= Pull, PUS = 22K Ohm Pull up, HYS = Enable
        IOMUXC->SW_PAD_CTL_PAD[PIN24_PAD] = 0x1F8B0;
        IOMUXC->SW_PAD_CTL_PAD[PIN25_PAD] = 0x1F8B0;
    }else {
        // SRE = Enable, DSE = R0/6, SPEED = max(200MHz), ODE = Enable, PKE = Enable, PUE= Pull, PUS = 22K Ohm Pull up, HYS = Enable
        // DSE = R0/7 yapmak icin 0x1F8F9
        IOMUXC->SW_PAD_CTL_PAD[PIN24_PAD] = 0x1F8F1;
        IOMUXC->SW_PAD_CTL_PAD[PIN25_PAD] = 0x1F8F1;
    }

    Wire3->MCR |=  LPI2C_MCR_MEN_MASK; // Master Logic Enable
}

void Wire3_Transmit(uint16_t* commands, uint8_t command_count) {
    for(int i = 0; i < command_count; i++) {
        xQueueSend(Wire3_TX_QUEUE, (commands + i), portMAX_DELAY);
    }

    Wire3->MIER |= LPI2C_MIER_TDIE_MASK; //Transmit Data Interrupt Enable
}

uint8_t Wire3_Receive(void) {
    uint8_t c;
    xQueueReceive(Wire3_RX_QUEUE, &c, portMAX_DELAY);
    return c;
}

void LPI2C4_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    //TX interrupt
    if(Wire3->MSR & LPI2C_MSR_TDF_MASK) {
        uint8_t space = 4 - (Wire3->MFSR & LPI2C_MFSR_TXCOUNT_MASK);
        uint16_t c;
        while(space--) {
            if(xQueueReceiveFromISR(Wire3_TX_QUEUE, &c, &xHigherPriorityTaskWoken) == pdPASS) {
                Wire3->MTDR = c;
            } else {
                Wire3->MIER &= ~LPI2C_MIER_TDIE_MASK;
                break;
            }
        }
    }

    if(Wire3->MSR & LPI2C_MSR_RDF_MASK) {
        while(!(Wire3->MRDR & LPI2C_MRDR_RXEMPTY_MASK)) {
            uint8_t c = (uint8_t)(Wire3->MRDR & 0xFF);
            xQueueSendFromISR(Wire3_RX_QUEUE, &c, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}