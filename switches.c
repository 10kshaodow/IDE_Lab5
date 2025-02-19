#include "switches.h"

void Switch1_Init(void)
{
	// configure PortPin for Switch1 as port I/O 
	P1->SEL0 &= ~BIT1 ; // select Port1 Bit1 for use as port I / O
	P1->SEL1 &= ~BIT1 ; // select Port1 Bit1 for use as port I / O

	// configure as input
	P1->DIR &= ~BIT1 ; // set BIT 1 as input
	
	P1->REN |= BIT1;
	P1->OUT |= BIT1;
              
}

void Switch2_Init(void)
{
    // configure PortPin for Switch2 as port I/O 
	P1->SEL0 &= ~BIT4 ; // select Port1 Bit4 for use as port I / O
	P1->SEL1 &= ~BIT4 ; // select Port1 Bit4 for use as port I / O

	// configure as input
	P1->DIR &= ~BIT4 ; // set BIT 4 as input

	P1->REN |= BIT4;
	P1->OUT |= BIT4;

}

//------------Switch_Input------------
// Read and return the status of Switch1
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch1_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	// check if pressed
	if ((P1->IN & BIT1) == 0) {
		retVal = TRUE;
	}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}

//------------Switch_Input------------
// Read and return the status of Switch2
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch2_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	// check if pressed
	if ((P1->IN & BIT4) == 0) {
		retVal = TRUE;
	}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}
