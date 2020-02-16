#include "GPIO.h"

/**
 * Initializes GPIOA to alternate function mode on Part A Pin 0
 * */
void GPIOA_Init(void){
	//Enable clock for GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//Select AF mode for Port A Pin 0
	GPIOA->MODER &= ~(0x03);
	GPIOA->MODER |= 2;
	
	//Select AF1 for Port A Pin 0
	GPIOA->AFR[0] &= ~(0x000F);
	GPIOA->AFR[0] |= 0x01;
	
	//Select AF mode for Port A Pin 0
	GPIOA->MODER &= ~(0x03 << 2);
	GPIOA->MODER |= (2 << 2);
	
	//Select AF1 for Port A Pin 0
	GPIOA->AFR[0] &= ~(0x0F << 4);
	GPIOA->AFR[0] |= 0x01 << 4;
}