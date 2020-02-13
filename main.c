#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32l476xx.h"
#include "TIM.h"
#include "SysClock.h"
#include "GPIO.h"

#define ITERATIONS         (1000)
#define MAX_LOOPS          (40000000)
#define ONE_HUNDRED_MS     (200000)


int main(void)
{

		// Board Initialization 
		System_Clock_Init(); // Switch System Clock = 80 MHz
		TIM2_Init();
		GPIOA_Init();
	
		// Variable Initialization
   
		while(1){
		}
}

