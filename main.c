#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32l476xx.h"
#include "TIM.h"
#include "SysClock.h"
#include "GPIO.h"
#include "UART.h"
#include "recipes.h"
#define ITERATIONS         (1000)
#define MAX_LOOPS          (40000000)
#define ONE_HUNDRED_MS     (200000)
#define STRING_SIZE				 (256)

int interruptCount = 0;
uint8_t s[STRING_SIZE];
uint8_t input[STRING_SIZE];

int main(void)
{

		// Board Initialization 
		System_Clock_Init(); // Switch System Clock = 80 MHz
		TIM2_Init();
		TIM5_Init();
		GPIOA_Init();
		UART2_Init();
	
		
		while(1){
		// Variable Initialization
			sprintf((char*) s, ">");
			USART_Write(USART2, s, strlen((char*)s));
	
			USART_Read_Str(USART2, input, STRING_SIZE);
			sprintf((char*) s, "Text entered: %s\r\n", input);
			USART_Write(USART2, s, strlen((char*)s));
		}
		

}

void TIM5_IRQHandler(void){
	TIM5->SR &= ~TIM_SR_UIF;
	TIM5->CNT = 0;
	
	interruptCount++;
	sprintf((char*) s, "Interrupt count: %d\r\n", interruptCount);
	
	
			// Iterator variable
    

    
    int servo_2_wait_cycles = 0;
    int servo_2_start_loop = 0;
    int servo_2_additional_loops = 0;

    unsigned char current_recipe_command = 0x00;

		// Variable should be controlled by UART, "P" or "p" should set this variable to 0 and "C", "B", "c" amd "b" should set to 1
		if (servo_1_state == State_Moving && servo_1_wait_time > 0 )
		{
				servo_1_wait_time--;				
			
		}
		else
			{
			servo_1_state = State_At_Position;
			if(recipe_1_exec && servo_1_state == State_At_Position)
			{
					// Servo 1 State Changes
					if (servo_1_wait_cycles==0)
					{
							current_recipe_command = recipe1[servo_1_lcv];
							switch(current_recipe_command & OPCODE)
							{
									case SHIFT:
											if(move_servo_to_position((uint32_t*) &TIM2->CCR1, servo_1_position + (current_recipe_command & PARAMETER) ))
											{
													servo_1_position += (current_recipe_command & PARAMETER);
													servo_1_state = State_Moving;
													
													set_servo_1_wait_time(abs((current_recipe_command & PARAMETER* 2)));
											}
											break;
									case MOV:
											// Move servo 1 to position based on the parameter

											if(move_servo_to_position((uint32_t*) &TIM2->CCR1, current_recipe_command & PARAMETER))
											{
													int wait_time = abs( (servo_1_position - (current_recipe_command & PARAMETER)) +(servo_1_position - (current_recipe_command & PARAMETER) )) - 1;
													
													if (wait_time)
													{
													servo_1_position = (current_recipe_command & PARAMETER);
													servo_1_state = State_Moving;
													servo_1_wait_time = wait_time;
													}
													
													//set_servo_1_wait_time(temp_wait);
											}
											break;

									case WAIT:
											// The switch statement will not be processed for the value of the parameter * (1/10) ms
											servo_1_wait_cycles = current_recipe_command & PARAMETER;
											break;
									case LOOP:
											servo_1_start_loop = servo_1_lcv;
											servo_1_additional_loops = current_recipe_command & PARAMETER;
											break;
									case END_LOOP:
											if (servo_1_additional_loops > 0)
											{
													servo_1_lcv = servo_1_start_loop;
													servo_1_additional_loops -= 1;
											}
											break;
									case END_RECIPE:
											// Recipe will not execute any further until this variable is set to 1 again
											recipe_1_exec = 0;
											servo_1_state = State_Recipe_Ended;
											break;

									default:
											break;
							}
							servo_1_lcv++;
					}
					else
					{
							servo_1_wait_cycles--;
					}
			}
		}
		// Variable should be controlled by UART, "P" or "p" should set this variable to 0 and "C", "B", "c" amd "b" should set to 1
		/*
		if(recipe_2_exec && servo_2_state == State_At_Position)
		{
				// Servo 2 State Changes
				if (servo_2_wait_cycles==0)
				{
						current_recipe_command = recipes[1][servo_2_lcv];
						switch(current_recipe_command & OPCODE)
						{
								case SHIFT:
										if(move_servo_to_position((uint32_t*) &TIM2->CCR2, servo_2_position + (current_recipe_command & PARAMETER) ))
										{
												servo_2_position += (current_recipe_command & PARAMETER);
												servo_2_state = State_Moving;
										}
										break;
								case MOV:
										// Move servo 1 to position based on the parameter
										if(move_servo_to_position((uint32_t*) &TIM2->CCR2, current_recipe_command & PARAMETER))
										{
												servo_2_position = (current_recipe_command & PARAMETER);
												servo_2_state = State_Moving;
										}
										
										break;

								case WAIT:
										// The switch statement will not be processed for the value of the parameter * (1/10) ms
										servo_2_wait_cycles = current_recipe_command & PARAMETER;
										break;
								case LOOP:
										servo_2_start_loop = servo_2_lcv + 1;
										servo_2_additional_loops = current_recipe_command & PARAMETER;
										break;
								case END_LOOP:
										if (servo_2_additional_loops > 0)
										{
												servo_2_lcv = servo_1_start_loop;
												servo_2_additional_loops -= 1;
										}
										break;
								case END_RECIPE:
										// Recipe will not execute any further until this variable is set to 1 again
										recipe_2_exec = 0;
										break;

								default:
										break;
						}
						servo_2_lcv++;
				}
				else
				{
						servo_2_wait_cycles--;
				}
		}
    */
		
	//USART_Write(USART2, s, STRING_SIZE);
}