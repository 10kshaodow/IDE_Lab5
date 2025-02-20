#include "msp.h"
#include "interrupts.h"

void Port1_IRQHandler(void)
{
    if(P1->IFG & BIT1)
    {
        P1->IFG &= ~BIT1; // clear interrupt flag
    }

    if (P1->IFG & BIT4)
    {
        P1->IFG &= ~BIT4; // clear interrupt flag
    }
}