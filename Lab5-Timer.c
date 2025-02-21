/*
* Rochester Institute of Technology
* Department of Computer Engineering
* CMPE 460  Interfacing Digital Electronics
* LJBeato
* 1/14/2021
*
* Filename: main_timer_template.c
*/
#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "switches.h"
#include "led.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"

#define RED			BIT0
#define GREEN 	    BIT1
#define BLUE		BIT2
#define CYAN		(GREEN|BLUE)
#define MAGENTA     (RED|BLUE)
#define YELLOW	    (RED|GREEN)
#define WHITE		(RED|GREEN|BLUE)

extern uint32_t SystemCoreClock;

// these are not used by the timer

uint16_t line[128];
BOOLEAN g_sendData = FALSE;

int colorIndex = 0;
BYTE colors[7] = { RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE };
int length = 7;
BYTE currentcolor;


BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;

unsigned long MillisecondCounter = 0;
//

//  I/O interrupt pin setup
//
// DIR     SEL0/SEL1    IE    IES     Port Mode
//  0          00        0     0       Input, rising edge trigger
//  0          00        0     1       Input, falling edge trigger, interrupt
//  0          00        1     0       Input, rising edge trigger, interrupt
//  0          00        1     1       Input, falling edge trigger, interrupt
//

void Switch1_Interrupt_Init(void)
{
	// disable interrupts
	DisableInterrupts();
	// initialize the Switch as per previous lab
	Switch1_Init();
	
 
	
	//7-0 PxIFG RW 0h Port X interrupt flag
	//0b = No interrupt is pending.
	//1b = Interrupt is pending.
	// clear flag1 (reduce possibility of extra interrupt)	
  	P1->IFG &= ~BIT1; // clear interrupt flag 

	//7-0 PxIE RW 0h Port X interrupt enable
	//0b = Corresponding port interrupt disabled
	//1b = Corresponding port interrupt enabled	
	// arm interrupt on  P1.1	
  	P1->IE |= BIT1; // enable interrupt 

	//7-0 PxIES RW Undefined Port X interrupt edge select
  	//0b = PxIFG flag is set with a low-to-high transition.
  	//1b = PxIFG flag is set with a high-to-low transition
	// now set the pin to cause falling edge interrupt event
	// P1.1 is falling edge event
  	P1->IES |= BIT1; // set interrupt to trigger on falling edge 
	
	// now set the pin to cause falling edge interrupt event
  NVIC_IPR8 = (NVIC_IPR8 & 0x00FFFFFF)|0x40000000; // priority 2
	
	// enable Port 1 - interrupt 35 in NVIC	
  NVIC_ISER1 = 0x00000008;  
	
	// enable interrupts  (// clear the I bit	)
  EnableInterrupts();              
	
}
void Switch2_Interrupt_Init(void)
{
	// disable interrupts
	DisableInterrupts();
	
	// initialize the Switch as per previous lab
	Switch2_Init();
	
	// now set the pin to cause falling edge interrupt event
	// P1.4 is falling edge event
	P1->IES |= BIT4; // set interrupt to trigger on falling edge
  
	// clear flag4 (reduce possibility of extra interrupt)
  	P1->IFG &= ~BIT4; // clear interrupt flag 
  
	// arm interrupt on P1.4 
	P1->IE |= BIT4; // enable interrupt    
	
	// now set the pin to cause falling edge interrupt event
  NVIC_IPR8 = (NVIC_IPR8&0x00FFFFFF)|0x40000000; // priority 2
	
	// enable Port 1 - interrupt 35 in NVIC
  NVIC_ISER1 = 0x00000008;         
	
	// enable interrupts  (// clear the I bit	)
  EnableInterrupts();              
	
}
// PORT 1 IRQ Handler
// LJBeato
// Will be triggered if any pin on the port causes interrupt
//
// Derived From: Jonathan Valvano


void PORT1_IRQHandler(void) // main purpose is to see where the interrupt came from and we can handle it respective values. 
{
	float numSeconds = 0.0;
	char temp[32];

	// First we check if it came from Switch1
  	if(P1->IFG & BIT1)  // we start a timer to toggle the LED1 1 second ON and 1 second OFF
	{
		// acknowledge P1.1 is pressed, by setting BIT1 to zero - remember P1.1 is switch 1
		// clear flag, acknowledge
    	P1->IFG &= ~BIT1; // clear interrupt flag
    	//start timer
		//
		if(Timer1RunningFlag == FALSE){
			Timer1RunningFlag = TRUE;
			
		}else{
			Timer1RunningFlag = FALSE;

		}
	}
		// Now check to see if it came from Switch2
  		if(P1->IFG & BIT4)
		{
			// acknowledge P1.4 is pressed, by setting BIT4 to zero - remember P1.4 is switch 2
			// clear flag4, acknowledge
			P1->IFG &= ~BIT4; // clear interrupt flag
			if(Timer2RunningFlag == FALSE){
				Timer2RunningFlag = TRUE;
				
			}else{
				Timer2RunningFlag = FALSE;
			}

			
			while(Timer2RunningFlag == TRUE)
			{
				LED2_Off(currentcolor);
				currentcolor = colors[colorIndex];
				LED2_On(currentcolor);
				while (MillisecondCounter % 500 != 0);
				colorIndex = (colorIndex + 1) % length;
				if(Timer2RunningFlag == FALSE){break;}
			}
			
			if(Timer2RunningFlag == FALSE)
			{
				LED2_Off(currentcolor);
				numSeconds = MillisecondCounter;
				MillisecondCounter = 0;
				numSeconds = numSeconds/1000; // might neeed to make for loop in putNumU
				uart0_put("\r\nSwitch2 was held for: ");
				uart0_putnumU((int)numSeconds);
				uart0_put(" Seconds\r\n");
				
			}
		
	}
}		


//
// Interrupt Service Routine for Timer32-1
//
//
//
void Timer32_1_ISR(void)
{
	if( Timer1RunningFlag ){
		if (LED1_State() ) // toggle 
		{
			LED1_On();
		}
		else LED1_Off();
	}
}

//
// Interrupt Service Routine
//
//
//
void Timer32_2_ISR(void)
{
if( Timer2RunningFlag ){
		MillisecondCounter++;
}
	
}


//
// main
//
//
//
int main(void){
	//initializations
	uart0_init();
	uart0_put("\r\nLab5 Timer demo\r\n");
	// Set the Timer32-2 to 2Hz (0.5 sec between interrupts)
	Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/2, T32DIV1); // initialize Timer A32-1;
        ;
        
	// Setup Timer32-2 with a .001 second timeout.
	// So use DEFAULT_CLOCK_SPEED/(1/0.001) = SystemCoreClock/1000
	Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock/1000, T32DIV1); // initialize Timer A32-1;
    
	Switch1_Interrupt_Init();
	Switch2_Interrupt_Init();
	LED1_Init();
	LED2_Init();
	EnableInterrupts();
  while(1)
	{
    WaitForInterrupt();
  }
}
