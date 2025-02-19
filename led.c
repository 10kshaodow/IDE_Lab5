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

void LED2_On(void) {
    P2->OUT |= BIT2; //blue
}

void LED2_Off(void) {
    P2->OUT &= ~BIT2;
}


void LED1_Off(void) {
    P1->OUT &= ~BIT0;
}
