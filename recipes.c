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
EventsE user_command_parse(char input)
{
	EventsE new_event_status;
	switch(input)
	{
		case 'P':
		case 'p':
			new_event_status = User_Paused_Recipe;
			break;
		case 'C':
		case 'c':		
			new_event_status = User_Continued_Recipe;
			break;
		case 'R':
		case 'r':
			new_event_status = User_Entered_Right;
			break;
		case 'L':
		case 'l':
			new_event_status = User_Entered_Left;
			break;
		case 'N':
		case 'n':
			new_event_status = User_Entered_No_Op;
			break;
		case 'B':
		case 'b':
			new_event_status = User_Begin_Restart_Recipe;
			break;
		default:
			new_event_status = User_Entered_Invalid_Recipe;
			break;
	}
	return new_event_status;
	
}

void event_command_parse(EventsE generated_event, ServoS* servo)
{
	switch(generated_event)
	{
		case User_Paused_Recipe:
			if (servo->servo_state != State_Recipe_Ended && servo->servo_status == Status_Running)
			{
				servo->servo_status = Status_Paused;
			}
			break;
		case User_Continued_Recipe:
			if (servo->servo_state != State_Recipe_Ended && servo->servo_status == Status_Paused)
			{
				servo->servo_status = Status_Running;
			}
			break;
		case User_Entered_Left:
	
				if (servo->servo_status != Status_Running && move_servo_to_position(servo->servo_ccr, servo->servo_position +1))
				{
					servo->servo_position++;
					servo->servo_state = State_Moving;	
					servo->servo_wait_time += 2;
					servo->recipe_exec = 1;
				}
				break;
				
		
		case User_Entered_Right:
			if (servo->servo_status != Status_Running && move_servo_to_position(servo->servo_ccr, servo->servo_position -1))
				{
					servo->servo_position--;
					servo->servo_state = State_Moving;	
					servo->servo_wait_time += 2;
					servo->recipe_exec = 1;
				}
			break;
		case User_Begin_Restart_Recipe:
			if (servo->servo_status != Status_Running)
			{
				servo->servo_lcv = 0;
				servo->servo_status = Status_Running;
				servo->recipe_exec = 1;
			}
			break;
		// If a no op is received, handle it the same as invalid
		case User_Entered_Invalid_Recipe:
		case User_Entered_No_Op:
	
		default:
			break;
	}
}
void init_servo(ServoS* servo)
{
	
	
	
	servo->servo_recipe = recipe1;
	servo->servo_wait_cycles = 0;
	servo->servo_start_loop = 0;
	servo->servo_additional_loops = 0;
	servo->recipe_exec = 1;
	servo->servo_position = 0;
	servo->servo_lcv =  0;
	servo->servo_wait_time = 0;
	servo->servo_status = Status_Running;
	servo->servo_state = State_At_Position;
	servo->servo_events = User_Paused_Recipe;
	
	
	
}
void recipe_parse( uint32_t *servo, unsigned char *recipe)
{
    
}

