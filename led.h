#ifndef LED_H
#define LED_H

#include "msp.h"
#include "Common.h"

// Function Prototypes
void LED1_Init(void);
void LED2_Init(void);
void LED1_On(void);
void LED1_Off(void);
void LED2_On(BYTE x); 
void LED2_Off(BYTE x); 
BOOLEAN LED1_State(void);
BOOLEAN LED2_State(void);



#endif /* LED_H */
