#include<stdlib.h>
#include<stdio.h>
#include "msp.h"
#include "uart.h"			// you will need to create this file
#include "led.h"
#include "switches.h"
#include "Timer32.h"
#include "interrupts.h"


#define BAUD_RATE 9600      //default baud rate 
#define CHAR_COUNT 35      //change this to modify the max. permissible length of a sentence


int main(){
	uart0_init();
	LED1_Init	();
	LED2_Init();
	Switch1_Init();
	Switch2_Init();
	//Timer32_1_Init();
	while(0){
	//Part 1 - LED cycling
		if(Switch1_Pressed()== TRUE){
			
		}
		
		
	}
	
	
	
	
	
	return 0;
}  
