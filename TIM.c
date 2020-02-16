#include "TIM.h"
#include <stdbool.h>

/**
 * Initializes the timer. Has a prescaler that scales back the timer frequency to 2MHZ. Enables input capture mode with interrupts.
 * */
void TIM2_Init(void){
	//Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    TIM2->PSC = 7999;
	
	//disable counter
	TIM2->CR1 &= ~(TIM_CR1_CEN);
    TIM2->EGR |= TIM_EGR_UG;
	
	//period of 20ms
	TIM2->ARR = 200;
	
	//duty cycle of 0.4ms
	//Channel 1
	TIM2->CCR1 = 4;
	//Channel 2
	TIM2->CCR2 = 50;
	
	//PWM mode 2
	//Channel 1
	TIM2->CCMR1 |= 0x06 << 4;
	//Channel 2
	TIM2->CCMR1 |= 0x06 << 12;
	
	//enable preload
	//Channel 1
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	//Channel 2
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
	
	//enable output
	//Channel 1
	TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S);
	//Channel 2
	TIM2->CCMR1 &= ~(TIM_CCMR1_CC2S);

	//enable auto-reload preload
	TIM2->CR1 |= TIM_CR1_ARPE;
	
	//enable capture compare
	//Channel 1
	TIM2->CCER |= TIM_CCER_CC1E;
	//Channel 2
	TIM2->CCER |= TIM_CCER_CC2E;
	
	//enable counter
	TIM2->CR1 |= TIM_CR1_CEN;
  TIM2->EGR |= TIM_EGR_UG;
}

void TIM6_Init(void){
	//1ms period
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
	TIM6->PSC |= 79999;
	
	//disable counter
	TIM6->CR1 &= ~(TIM_CR1_CEN);
    TIM6->EGR |= TIM_EGR_UG;
	
	//generate interrupt every 100ms
	TIM6->ARR = 100;
	TIM6->CR1 |= TIM_CR1_ARPE;
	
	TIM6->DIER |= TIM_DIER_UIE;
	
	TIM6->CR1 |= TIM_CR1_URS;
	
	TIM6->CR1 |= TIM_CR1_CEN;
	TIM6->EGR |= TIM_EGR_UG;
}

/**
 * Stops the timer for timer 2.
 * */
void TIM2_Stop(void){
	TIM2->CR1 &= ~(TIM_CR1_CEN);
    TIM2->EGR |= TIM_EGR_UG;
}

/**
 * Starts the timer for timer 2.
 * */
void TIM2_Start(void){
	TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->EGR |= TIM_EGR_UG;
}

/**
 * Sets the value of the timer to 0 for timer 2.
 * */
void TIM2_Reset(void){
	TIM2->CNT = 0;
}