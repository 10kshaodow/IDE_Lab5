/*
 * File:uart.c
 */

#include "msp.h"
#include "uart.h"  // you need to create this file with the function prototypes
#include "Common.h"  // from Lab1 code

#define BAUD_RATE 9600      //default baud rate 
extern uint32_t SystemCoreClock;  // clock rate of MCU


void uart0_init(){
	int Mod;
	//Set the UART to RESET state (set bit0 of EUSCI_A0->CTLW0 register to '1'
	EUSCI_A0->CTLW0 |= BIT0;

	EUSCI_A0->CTLW0 = 0x0080; 

	// set CTLW0 - hold logic and configure clock source to SMCLK
	
	EUSCI_A0->CTLW0 |= (BIT7 | BIT6);
	

	// baud rate
	// N = clock/baud rate = clock_speed/BAUD_RATE
	// set BRW register
	Mod = SystemCoreClock / BAUD_RATE; // Mod = UART module clock / baud rate
  EUSCI_A0->BRW = Mod; // Set integer part of the baud rate divisor
  EUSCI_A0->MCTLW = 0; // No fractional modulation for simplicity
	

	// P1.3 = TxD
	// P1.2 = RxD
	// we will be using P1.2, P1.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
	// set SEL0, SEL1 appropriately
	P1->SEL0 |= (BIT2 | BIT3); // Set P1.2 and P1.3 to use UART functionality
  P1->SEL1 &= ~(BIT2 | BIT3);
	


	// CTLW0 register - release from reset state
	EUSCI_A0->CTLW0 &= ~BIT0; // Clear UCSWRST to activate UART

	

	// disable interrupts (transmit ready, start received, transmit empty, receive full)	
	// IE register;      
	EUSCI_A0->IE &= ~(BIT0 | BIT1); // Disable RXIE (bit 0) and TXIE (bit 1)

}
void uart2_init(){
	int Mod;
	//Set the UART to RESET state (set bit0 of EUSCI_A2->CTLW0 register to '1'
	EUSCI_A2->CTLW0 |= BIT0;

	EUSCI_A2->CTLW0 = 0x0080; 

	// set CTLW0 - hold logic and configure clock source to SMCLK
	
	EUSCI_A2->CTLW0 |= (BIT7 | BIT6);
	

	// baud rate
	// N = clock/baud rate = clock_speed/BAUD_RATE
	// set BRW register
	Mod = SystemCoreClock / BAUD_RATE; // Mod = UART module clock / baud rate
  EUSCI_A2->BRW = Mod; // Set integer part of the baud rate divisor
  EUSCI_A2->MCTLW = 0; // No fractional modulation for simplicity
	

	// P3.2 = RxD
	// P3.3 = TxD
	// we will be using P3.2, P3.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
	// set SEL0, SEL1 appropriately
	P3->SEL0 |= (BIT2 | BIT3); // Set P1.2 and P1.3 to use UART functionality
  P3->SEL1 &= ~(BIT2 | BIT3);
	


	// CTLW0 register - release from reset state
	EUSCI_A2->CTLW0 &= ~BIT0; // Clear UCSWRST to activate UART

	

	// disable interrupts (transmit ready, start received, transmit empty, receive full)	
	// IE register;      
	EUSCI_A2->IE &= ~(BIT0 | BIT1); // Disable RXIE (bit 0) and TXIE (bit 1)

}

BYTE uart0_getchar(){
	BYTE inChar;
	// Wait for data
	// IFG register
	
	while(!((EUSCI_A0 -> IFG) & BIT0)){ //wait for IFG to read as 1 
	}
	// read character and store in inChar variable
	// RXBUF register
	inChar = EUSCI_A0 -> RXBUF;
		
	//Return the 8-bit data from the receiver 
	return(inChar);

}

void uart0_putnumU(int i){ //designed to only take a single digit, must be used in loop for large numbers.
	char ch;
	if (i >= 0 && i <= 9) {
        ch = i + '0';  // the ASCII value for any digit is simply 48('0') + digit
    } else {
        ch = '?';  // Placeholder for non-digit values
    }
	
	// Wait until transmission of previous bit is complete 
	// IFG register
	while(!((EUSCI_A0 -> IFG) & BIT1)) { 
        // Busy-wait loop for UCTXIFG bit to be set (indicates UART is not busy)
    }
	 EUSCI_A0 -> TXBUF = ch; // Load the character into the TX buffer

	}
void uart2_putnumU(int i){ //designed to only take a single digit, must be used in loop for large numbers.
	char ch;
	if (i >= 0 && i <= 9) {
        ch = i + '0';  // the ASCII value for any digit is simply 48('0') + digit
    } else {
        ch = '?';  // Placeholder for non-digit values
    }
	
	// Wait until transmission of previous bit is complete 
	// IFG register
	while(!((EUSCI_A2 -> IFG) & BIT1)) { 
        // Busy-wait loop for UCTXIFG bit to be set (indicates UART is not busy)
    }
	 EUSCI_A2 -> TXBUF = ch; // Load the character into the TX buffer

	}

BYTE uart2_getchar(){
	BYTE inChar;
	// Wait for data
	// IFG register
	
	while(!((EUSCI_A2 -> IFG) & BIT0)){ //wait for IFG to read as 1 
	}
	// read character and store in inChar variable
	// RXBUF register
	inChar = EUSCI_A2 -> RXBUF;
		
	//Return the 8-bit data from the receiver 
	return(inChar);

}

void uart0_putchar(char ch)
{
	// Wait until transmission of previous bit is complete 
	// IFG register
	while(!((EUSCI_A0 -> IFG) & BIT1)) { 
        // Busy-wait loop for UCTXIFG bit to be set (indicates UART is not busy)
    }
	// send ch character to uart
		 EUSCI_A0 -> TXBUF = ch; // Load the character into the TX buffer
	// TXBUF register 
}
void uart2_putchar(char ch)
{
	// Wait until transmission of previous bit is complete 
	// IFG register
	while(!((EUSCI_A2 -> IFG) & BIT1)) { 
        // Busy-wait loop for UCTXIFG bit to be set (indicates UART is not busy)
    }
	// send ch character to uart
		 EUSCI_A2 -> TXBUF = ch; // Load the character into the TX buffer
	// TXBUF register 
}
BOOLEAN uart0_dataAvailable(){
BOOLEAN go = FALSE ;
// if a character is available , set go = TRUE
/* your code here */ 
	if ((EUSCI_A0->IFG & BIT0) != 0) {
        go = TRUE;
}
return go ;
}
BOOLEAN uart2_dataAvailable(){
BOOLEAN go = FALSE ;
// if a character is available , set go = TRUE
if ((EUSCI_A2->IFG & BIT0) != 0) {
        go = TRUE;
}
return go ;
}
void uart0_put(char *ptr_str)
{
	while(*ptr_str != 0)
		uart0_putchar(*ptr_str++);
}
void uart2_put(char *ptr_str)
{

	while(*ptr_str != 0)
		uart2_putchar(*ptr_str++);
}
