#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32l476xx.h"
#include "TIM.h"
#include "SysClock.h"
#include "GPIO.h"
#include "UART.h"

#define ITERATIONS         (1000)
#define MAX_LOOPS          (40000000)
#define ONE_HUNDRED_MS     (200000)
#define STRING_SIZE				 (256)

int interruptCount = 0;
uint8_t s[STRING_SIZE];

int main(void)
{

		// Board Initialization 
		System_Clock_Init(); // Switch System Clock = 80 MHz
		TIM2_Init();
		TIM5_Init();
		GPIOA_Init();
		UART2_Init();
	
		
	
		// Variable Initialization
   
		while(1){
		}
}

void TIM5_IRQHandler(void){
	TIM5->SR &= ~TIM_SR_UIF;
	TIM5->CNT = 0;
	
	interruptCount++;
	sprintf((char*) s, "Interrupt count: %d\r\n", interruptCount);
	
	
	USART_Write(USART2, s, STRING_SIZE);
}