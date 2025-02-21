#include "LED.h"

void LED1_Init(void) {
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;
    P1->DS |= BIT0;
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;
}

void LED2_Init(void) {
    P2->SEL0 &= ~(BIT0 | BIT1 | BIT2);
    P2->SEL1 &= ~(BIT0 | BIT1 | BIT2);
    P2->DS |= (BIT0 | BIT1 | BIT2);
    P2->DIR |= (BIT0 | BIT1 | BIT2);
    P2->OUT &= ~(BIT0 | BIT1 | BIT2);
}

void LED1_On(void) {
    P1->OUT |= BIT0; //red
}

void LED2_On(BYTE x) {
    P2->OUT |= x; 
}

void LED2_Off(BYTE x) {
    P2->OUT &= ~x;
}


void LED1_Off(void) {
    P1->OUT &= ~BIT0;
}

BOOLEAN LED1_State(void) {
static BOOLEAN state = FALSE; 

    // Toggle the state
    state = !state;
    
    // Return the current state
    return state;
}

