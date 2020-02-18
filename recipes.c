#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "recipes.h"
#include "TIM.h"

// Global State value
// Initial State as paused

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

int get_servo_1_wait_time(void)
{
	return servo_1_wait_time;
}

int get_servo_2_wait_time(void)
{
	return servo_2_wait_time;
}

void set_servo_1_wait_time(int value)
{
	servo_1_wait_time = value;
}

void set_servo_2_wait_time(int value)
{
	servo_2_wait_time = value;
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
            *servo = POSITION_0;
            break;
        case 1:
            *servo = POSITION_1;
            break;
        case 2:
            *servo = POSITION_2;
            break;
        case 3:
            *servo = POSITION_3;
            break;
        case 4:
            *servo = POSITION_4;
            break;
        case 5:
            *servo = POSITION_5;
            break;
        default:
            return 0;
    }
    return 1;
}

void recipe_parse( uint32_t *servo, unsigned char *recipe)
{
    
}

