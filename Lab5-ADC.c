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
#include "leds.h"
#include "switches.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
// The sprintf function seemed to cause a hange in the interrupt service routine.
// I think if we increase the HEAP size, it will work
// change to Heap_Size       EQU     0x00000200 in startup_msp432p401r_uvision.s

#define ADC_MAX 16384.0   // 14-bit ADC resolution
#define VREF 3.3          // Reference voltage in Volts

BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;

unsigned long MillisecondCounter = 0;

extern uint32_t SystemCoreClock;

volatile unsigned int adc_photocell = 0;
volatile unsigned int adc_temp = 0;
volatile BOOLEAN new_photocell_data = FALSE;
volatile BOOLEAN new_temp_data = FALSE;


// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
    adc_photocell = ADC_In(); // Read ADC value from channel A6 (P4.7)
    new_photocell_data = TRUE;
}


// Interrupt Service Routine for Timer32-2
void Timer32_2_ISR(void)
{
    adc_temp = ADC_In(); // Read ADC value from the TMP36 sensor
    new_temp_data = TRUE;
}



// main
int main(void)
{
	char temp[64];
	unsigned int analogIn = 0;
	//initializations
	uart0_init();
	uart0_put("\r\nLab5 ADC demo\r\n");

	LED1_Init();
	LED2_Init();
	Switch2_Init();
	ADC0_InitSWTriggerCh6();

	Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock/2, T32DIV1); // initialize Timer A32-1;
	Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock/2, T32DIV1); // initialize Timer A32-2;

	EnableInterrupts();
  	while(1)
	{
		for (;;) {
			if (new_photocell_data) {
				new_photocell_data = FALSE;
				sprintf(temp, "\r\nPhotocell -> Hex: %X | Dec: %u", adc_photocell, adc_photocell);
				uart0_put(temp);
			}
	
			if (new_temp_data) {
				new_temp_data = FALSE;
				// Convert ADC value to temperature
				double voltage = (adc_temp / ADC_MAX) * VREF;
				double temperature_C = (voltage * 1000.0 - 500.0) / 10.0;
				double temperature_F = (temperature_C * 9.0 / 5.0) + 32.0;
	
				sprintf(temp, "\r\nTMP36 -> Hex: %X | Dec: %u | Temp: %.2f C | %.2f F", adc_temp, adc_temp, temperature_C, temperature_F);
				uart0_put(temp);
			}
		}
  	}
}

