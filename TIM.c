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
	TIM2->CCR1 = 4;

	//PWM mode 2
	TIM2->CCMR1 |= 0x03 << 4;
	
	//enable preload
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	
	//enable output
	TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S);

	//enable auto-reload preload
	TIM2->CR1 |= TIM_CR1_ARPE;
	
	//enable capture compare
	TIM2->CCER |= TIM_CCER_CC1E;
	
	//enable counter
	TIM2->CR1 |= TIM_CR1_CEN;
  TIM2->EGR |= TIM_EGR_UG;
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