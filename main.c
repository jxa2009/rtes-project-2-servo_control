#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32l476xx.h"
#include "TIM.h"
#include "SysClock.h"
#include "GPIO.h"
#include "UART.h"
#include "LED.h"
#include "recipes.h"
#define ITERATIONS         (1000)
#define MAX_LOOPS          (40000000)
#define ONE_HUNDRED_MS     (200000)
#define STRING_SIZE				 (256)
#define INPUT_SIZE         (2)
int interruptCount = 0;
uint8_t s[STRING_SIZE];
uint8_t input[INPUT_SIZE+1];
int wait_time = 0;
int new_position = 0;
int loop_amount = 0;
unsigned char current_recipe_command = 0x00;
int main(void)
{

		// Board Initialization 
		System_Clock_Init(); // Switch System Clock = 80 MHz
		TIM2_Init();
		GPIOA_Init();
		UART2_Init();
		LED_Init();
		// Initialize a given servo with a pointer to the PWM that you change
		init_servo(&servo_1, recipe1, (uint32_t*) &(TIM2->CCR1));
		init_servo(&servo_2, recipe2, (uint32_t*) &(TIM2->CCR2));
		
		sprintf((char*) s, "Getting servos into position...\r\n");
		USART_Write(USART2, s, strlen((char*)s));
	
		//give servos time to position themselves before initiating the master timer
		for(int i = 0; i < 40000000; i++){
		}
		TIM5_Init();
		
		sprintf((char*) s, "Ready\r\n");
		USART_Write(USART2, s, strlen((char*)s));
		
		while(1){
			// Variable Initialization
			sprintf((char*) s, ">");
			USART_Write(USART2, s, strlen((char*)s));
	
			USART_Read_Str(USART2, input, INPUT_SIZE+1);
			sprintf((char*) s, "Text entered: %s\r\n", input);
			USART_Write(USART2, s, strlen((char*)s));
			
			// If there is an X in a command, the command is ignored
			if(!(input[0] == 'X' || input[1] == 'x' || input[0] == 'X' || input[1] == 'x'))
			{
				//Process command for servo 1
				servo_1.servo_events = user_command_parse((char) input[0]);
				event_command_parse(servo_1.servo_events, &servo_1);
				// Process command for servo 2
				servo_2.servo_events = user_command_parse((char) input[1]);
				event_command_parse(servo_2.servo_events, &servo_2);
			}
		}


}

void TIM5_IRQHandler(void){
	TIM5->SR &= ~TIM_SR_UIF;
	TIM5->CNT = 0;
	
	interruptCount++;
	//sprintf((char*) s, "Interrupt count: %d\r\n", interruptCount);

	current_recipe_command = 0x00;
	// Pressing c or b will change servo status to enter this condition
	if ( servo_1.servo_status != Status_Command_Error)
	{
		if (servo_1.servo_status == Status_Running)
		{
			// A MOV in the recipe will set the state to moving
			if (servo_1.servo_state == State_Moving && servo_1.servo_wait_time> 0 )
			{
				servo_1.servo_wait_time--;
			}
			else
			{
				// It should only enter this condition if the servo is stopped at a positoin
				servo_1.servo_state = State_At_Position;
				// This function will never be entered if its hit recipe_end. Must be stopped at a position
				if(servo_1.recipe_exec && servo_1.servo_state== State_At_Position)
				{
					// Servo 1 State Changes
					if (servo_1.servo_wait_cycles == 0)
					{
						// Grab the current recipe
						current_recipe_command = servo_1.servo_recipe[servo_1.servo_lcv];
						switch(current_recipe_command & OPCODE)
						{
								case SHIFT:
									
									
									//Attempts to move the servo left or right 
									new_position = (servo_1.servo_position + (current_recipe_command & PARAMETER)) % 6;
									
									if (move_servo_to_position( servo_1.servo_ccr, new_position))
									{		
										wait_time = abs( ( servo_1.servo_position - new_position) + (servo_1.servo_position - new_position) ) - 1;
										// These values should only change if it actaully moves
										servo_1.servo_position = new_position;
										servo_1.servo_state = State_Moving;
										servo_1.servo_wait_time =wait_time;
									}
									break;
									
								case MOV:
									new_position = (current_recipe_command & PARAMETER);
									
									// Move servo 1 to position based on the parameterss
									if( (new_position >= 0) && (new_position <= 5) && (move_servo_to_position(servo_1.servo_ccr, new_position )))
									{
										// Calculate how long to  wait based on how far its moving based on the current position
										wait_time = abs( ( servo_1.servo_position - (current_recipe_command & PARAMETER)) + (servo_1.servo_position - (current_recipe_command & PARAMETER) ) -1) ;
										if (wait_time)
										{
											// These values should only change if it actually moves
											servo_1.servo_position = (current_recipe_command & PARAMETER);
											servo_1.servo_state = State_Moving;
											servo_1.servo_wait_time = wait_time;
										}
									}
									else
									{
										servo_1.servo_status = Status_Command_Error;
									}
									break;

								case WAIT:
									// The switch statement will not be processed for the value of the parameter * (1/10) ms
									//new_position is just the amount of time to wait
								
									wait_time =  current_recipe_command & PARAMETER;
									if(wait_time <= 31 && wait_time >= 0)
									{
										servo_1.servo_wait_cycles = wait_time;
									}
									else
									{
										servo_1.servo_status = Status_Command_Error;
									}
									
									break;
								
								case LOOP:
									//Keep track of the start of the loop and how many times to be looping
									loop_amount = current_recipe_command & PARAMETER;
									if(servo_1.servo_additional_loops == 0 && loop_amount >= 0 && loop_amount <= 31)
									{
										servo_1.servo_start_loop = servo_1.servo_lcv;
										servo_1.servo_additional_loops = loop_amount;
									}
									else
									{
										servo_1.servo_status = Status_Nested_Error;
									}
									break;
								
								case END_LOOP:
									// If there still needs to be looping then move iterator back to the beginning of the loop and decrement
									if (servo_1.servo_additional_loops >0)
									{
										servo_1.servo_lcv = servo_1.servo_start_loop;
										servo_1.servo_additional_loops -= 1;
									}
									break;
										
								case END_RECIPE:
									// Recipe will not execute any further until this variable is set to 1 again
									servo_1.recipe_exec = 0;
									servo_1.servo_state = State_Recipe_Ended;
									servo_1.servo_status = Status_Paused;
									break;

								default:
									break;
						}
						//Increment iterator
						servo_1.servo_lcv++;
					}
					else
					{
						// Decrement counter for if you hit a wait in the recipe
						servo_1.servo_wait_cycles--;
					}
				}
			}
		}
	}
	
	set_green_led(servo_1.servo_status & 0x01);
	set_red_led(servo_1.servo_status & 0x02);
	
		// Pressing b will change servo status to enter this condition
	if ( servo_2.servo_status != Status_Command_Error)
	{
		if (servo_2.servo_status == Status_Running )
		{
			// A MOV in the recipe will set the state to moving
			if (servo_2.servo_state == State_Moving && servo_2.servo_wait_time> 0 )
			{
				servo_2.servo_wait_time--;
			}
			else
			{
				// It should only enter this condition if the servo is stopped at a positoin
				servo_2.servo_state = State_At_Position;
				// This function will never be entered if its hit recipe_end. Must be stopped at a position
				if(servo_2.recipe_exec && servo_2.servo_state== State_At_Position)
				{
					// Servo 2 State Changes
					if (servo_2.servo_wait_cycles == 0)
					{
						// Grab the current recipe
						current_recipe_command = servo_2.servo_recipe[servo_2.servo_lcv];
						switch(current_recipe_command & OPCODE)
						{
								case SHIFT:
									//Attempts to move the servo left or right 
									new_position = (servo_2.servo_position + (current_recipe_command & PARAMETER)) % 6;
									
									if (move_servo_to_position( servo_2.servo_ccr, new_position))
									{		
										wait_time = abs( ( servo_2.servo_position - new_position) + (servo_2.servo_position - new_position) ) - 1;
										// These values should only change if it actaully moves
										servo_2.servo_position = new_position;
										servo_2.servo_state = State_Moving;
										servo_2.servo_wait_time =wait_time;
									}
									break;
									
								case MOV:
									new_position = (current_recipe_command & PARAMETER);
									
									// Move servo 1 to position based on the parameterss
									if( (new_position >= 0) && (new_position <= 5) && (move_servo_to_position(servo_2.servo_ccr, new_position )))
									{
										// Calculate how long to  wait based on how far its moving based on the current position
										wait_time = abs( ( servo_2.servo_position - (current_recipe_command & PARAMETER)) + (servo_2.servo_position - (current_recipe_command & PARAMETER) )) -1 ;
										if (wait_time)
										{
											// These values should only change if it actually moves
											servo_2.servo_position = (current_recipe_command & PARAMETER);
											servo_2.servo_state = State_Moving;
											servo_2.servo_wait_time = wait_time;
										}
									}
									else
									{
										servo_2.servo_status = Status_Command_Error;
									}
									break;

								case WAIT:
									// The switch statement will not be processed for the value of the parameter * (1/10) ms
									//new_position is just the amount of time to wait
								
									wait_time =  current_recipe_command & PARAMETER;
									if(wait_time <= 31 && wait_time >= 0)
									{
										servo_2.servo_wait_cycles = wait_time;
									}
									else
									{
										servo_2.servo_status = Status_Command_Error;
									}
									
									break;
								
								case LOOP:
									//Keep track of the start of the loop and how many times to be looping
									loop_amount = current_recipe_command & PARAMETER;
									if(servo_2.servo_additional_loops == 0 && loop_amount >= 0 && loop_amount <= 31)
									{
										servo_2.servo_start_loop = servo_2.servo_lcv;
										servo_2.servo_additional_loops = loop_amount;
									}
									else
									{
										servo_2.servo_status = Status_Nested_Error;
									}
									break;
								
								case END_LOOP:
									// If there still needs to be looping then move iterator back to the beginning of the loop and decrement
									if (servo_2.servo_additional_loops >0)
									{
										servo_2.servo_lcv = servo_2.servo_start_loop;
										servo_2.servo_additional_loops -= 1;
									}
									break;
										
								case END_RECIPE:
									// Recipe will not execute any further until this variable is set to 1 again
									servo_2.recipe_exec = 0;
									servo_2.servo_state = State_Recipe_Ended;
									servo_2.servo_status = Status_Paused;
									break;

								default:
									break;
						}
						//Increment iterator
						servo_2.servo_lcv++;
					}
					else
					{
						// Decrement counter for if you hit a wait in the recipe
						servo_2.servo_wait_cycles--;
					}
				}
			}
		}
	}
		
	//USART_Write(USART2, s, STRING_SIZE);
}