#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "recipes.h"
#include "TIM.h"

/**
 * Change the index of the loop counter variable for servo 1 recipes, can be used for resetting 
 * Inputs:
 *      [int] index - Index for servo 1 loop counter variable to be set to
 * Outputs:
 *      None
 * */
void set_servo_1_lcv(int index)
{
    servo_1_lcv = index;
}

/**
 * Change the index of the loop counter variable for servo 2 recipes, can be used for resetting 
 * Inputs:
 *      [int] index - Index for servo 2 loop counter variable to be set to
 * Outputs:
 *      None
 * */
void set_servo_2_lcv(int index)
{
    servo_2_lcv = index;
}

/**
 * Function that moves a given servo to positions 0 to 5. Any others will fail.
 * Inputs:
 *          [uint32_t] *servo - pointer to a given servo (1 or 2)
 *          [int] positions - Number 0 to 5 to move the servo
 * Outputs:
 *          Returns 1 if successfully moved, 0 if failed
 * */
int move_servo_to_position( uint32_t *servo, int position)
{
    // Might have to change positions ? servo is pointer to uin32
    if (position < 0 || position > 5)
    {
        return 0;
    }
    switch(position)
    {
        case 0:
            servo = POSITION_0;
            break;
        case 1:
            servo = POSITION_1;
            break;
        case 2:
            servo = POSITION_2;
            break;
        case 3:
            servo = POSITION_3;
            break;
        case 4:
            servo = POSITION_4;
            break;
        case 5:
            servo = POSITION_5;
            break;
        default:
            return 0;
    }
    return 1;
}

void recipe_parse()
{
    // Iterator variable
    
    int servo_1_wait_cycles = 0;
    int servo_1_start_loop = 0;
    int servo_1_additional_loops = 0;
    
    int servo_2_wait_cycles = 0;
    int servo_2_start_loop = 0;
    int servo_2_additional_loops = 0;

    unsigned char current_recipe_command = 0x00;
    while(1)
    {
        // Variable should be controlled by UART, "P" or "p" should set this variable to 0 and "C", "B", "c" amd "b" should set to 1
        if(recipe_1_exec)
        {
            // Servo 1 State Changes
            if (servo_1_wait_cycles==0)
            {
                current_recipe_command = recipes[0][servo_1_lcv]
                switch(current_recipe_command && OPCODE)
                {

                    case SHIFT:
                        if(move_servo_to_position(TIM2->CCR1, servo_1_position + (current_recipe_command & PARAMETER) ))
                        {
                            servo_1_position += (current_recipe_command & PARAMETER)
                            servo_1_state = State_Moving;
                        }
                        break;
                    case MOV:
                        // Move servo 1 to position based on the parameter

                        if(move_servo_to_position(TIM2->CCR1, current_recipe_command & PARAMETER))
                        {
                            servo_1_position = (current_recipe_command & parameter);
                        }
                        break;

                    case WAIT:
                        // The switch statement will not be processed for the value of the parameter * (1/10) ms
                        servo_1_wait_cycles = current_recipe_command & PARAMETER;
                        break;
                    case LOOP:
                        servo_1_start_loop = servo_1_lcv + 1;
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
        // Variable should be controlled by UART, "P" or "p" should set this variable to 0 and "C", "B", "c" amd "b" should set to 1
        if(recipe_2_exec)
        {
            // Servo 2 State Changes
            if (servo_2_wait_cycles==0)
            {
                current_recipe_command = recipes[1][servo_2_lcv]
                switch(current_recipe_command && OPCODE)
                {
                    case SHIFT:
                        if(move_servo_to_position(TIM2->CCR2, servo_2_position + (current_recipe_command & PARAMETER) ))
                        {
                            servo_2_position += (current_recipe_command & PARAMETER)
                            servo_2_state = State_Moving;
                        }
                        break;
                    case MOV:
                        // Move servo 1 to position based on the parameter
                        if(move_servo_to_position(TIM2->CCR2, current_recipe_command & PARAMETER))
                        {
                            servo_2_position = (current_recipe_command & PARAMETER)
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
    }
}

