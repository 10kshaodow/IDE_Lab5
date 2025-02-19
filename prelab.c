#include "msp.h"

void PORT1_IRQHandler(void) {
    if (P1->IFG & BIT1) {  // Check if interrupt was triggered by P1.1 (SW1)
        P1->IFG &= ~BIT1;  // Clear interrupt flag
    }
    if (P1->IFG & BIT4) {  // Check if interrupt was triggered by P1.4 (SW2)
        P1->IFG &= ~BIT4;  // Clear interrupt flag
    }
}

void setup_SW1_interrupt() {
    P1->DIR &= ~BIT1;    // Set P1.1 as input
    P1->OUT |= BIT1;     // Enable pull-up resistor
    P1->REN |= BIT1;     // Enable pull-up resistor
    P1->IES |= BIT1;     // Set interrupt on falling edge
    P1->IFG &= ~BIT1;    // Clear any existing interrupt flag
    P1->IE |= BIT1;      // Enable interrupt for P1.1
    
    NVIC_EnableIRQ(PORT1_IRQn); // Enable PORT1 interrupts in NVIC
}


void setup_SW2_interrupt() {
    P1->DIR &= ~BIT4;    // Set P1.4 as input
    P1->OUT |= BIT4;     // Enable pull-up resistor
    P1->REN |= BIT4;     // Enable pull-up resistor
    P1->IES |= BIT4;     // Set interrupt on falling edge
    P1->IFG &= ~BIT4;    // Clear any existing interrupt flag
    P1->IE |= BIT4;      // Enable interrupt for P1.4
    
    NVIC_EnableIRQ(PORT1_IRQn); // Enable PORT1 interrupts in NVIC
}


void Timer32_init() {
    TIMER32_1->CONTROL |= BIT6; // Periodic mode
    TIMER32_1->CONTROL |= BIT3; // Prescaler /256 
    TIMER32_1->CONTROL &= ~BIT2;
    TIMER32_1->CONTROL |= BIT5; // Enable interrupt
    TIMER32_1->LOAD = 3000000;  // Load count for ~1s delay at 3MHz clock
    TIMER32_1->CONTROL |= BIT7; // Enable Timer32
}

void T32_INT1_IRQHandler(void) {
    TIMER32_1->INTCLR = 0; // Clear interrupt flag
}


void ADC_init() {

    ADC14->CTL0 &= ~ADC14_CTL0_ENC; // Clear enable conversion so that SHT0 can be modified below

    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_ON;  // Sample/Hold time and turn on ADC.  NOTE: Not sure which SHT value to use, just picked one at random
    ADC14->CTL1 = ADC14_CTL1_RES_2;  // 12-bit resolution
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_6;  // Select channel A6 (Input from P4.7)
    
    P4->SEL0 |= BIT7;  // Configure P4.7 for ADC
    P4->SEL1 |= BIT7;

    // Turn back on here
    ADC14->CTL0 |= ADC14_CTL0_ENC;  // re-enable conversion once registers are configured
}


uint16_t read_ADC_A6() {
    ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;  // Start conversion
    while (!(ADC14->IFGR0 & ADC14_IFGR0_IFG0));  // Wait for conversion to complete
    return ADC14->MEM[0];  // Read result from ADC14MEM0
}

