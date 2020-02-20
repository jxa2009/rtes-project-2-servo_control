#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "recipes.h"
#include "TIM.h"

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
/**
* Takes in an input and returns the event that is supposed to occur based on given input
* Inputs:
*				[char] input - character to be parsed into an event
* Output:
				Returns event based on given input
*/
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

/**
* Takes in an event that was generated and changes servo states/status based on the event
* Inputs:
*				[EventsE] generated_event - Event that is created by user input
*       [ServoS*] servo - Pointer to the servo that will have its read from
* Outputs:
*				[ServoS*] servo - Pointer to servo that will have some of its information changed based on event
* Returns:
				Nothing
*/
void event_command_parse(EventsE generated_event, ServoS* servo)
{
	switch(generated_event)
	{
		case User_Paused_Recipe:
			//Cannot pause if recipe ended, must be running
			if (servo->servo_state != State_Recipe_Ended && servo->servo_status == Status_Running)
			{
				servo->servo_status = Status_Paused;
			}
			break;
		// Cannot continue if the recipe ended, must be paused
		case User_Continued_Recipe:
			if (servo->servo_state != State_Recipe_Ended && servo->servo_status == Status_Paused)
			{
				servo->servo_status = Status_Running;
			}
			break;
		// Cannot turn left if servo is running 
		case User_Entered_Left:
	
				if (servo->servo_status != Status_Running && move_servo_to_position(servo->servo_ccr, servo->servo_position +1))
				{
					servo->servo_position++;
					servo->servo_state = State_Moving;	
					servo->servo_wait_time += 2;
					servo->recipe_exec = 1;
				}
				break;
		// Cannot turn right if servo is running
		case User_Entered_Right:
			if (servo->servo_status != Status_Running && move_servo_to_position(servo->servo_ccr, servo->servo_position -1))
				{
					servo->servo_position--;
					servo->servo_state = State_Moving;	
					servo->servo_wait_time += 2;
					servo->recipe_exec = 1;
				}
			break;
		// Cannot restart if servo is currently running
		case User_Begin_Restart_Recipe:
			if (servo->servo_status != Status_Running)
			{
				servo->servo_lcv = 0;
				servo->servo_status = Status_Running;
				servo->recipe_exec = 1;
			}
			break;
		// If a no op is received, handle it the same as invalid, does nothing
		case User_Entered_Invalid_Recipe:
		case User_Entered_No_Op:
	
		default:
			break;
	}
}

/**
* Initializes a servo to default settings
* Inputs:
				[ServoS*] servo - Pointer to servo struct
				[uint32_t*] timer - Pointer to channel for PWM in a timer
* Returns:
				Nothing
* */
void init_servo(ServoS* servo, unsigned char* recipe, uint32_t* timer)
{
	servo->servo_ccr = timer;
	servo->servo_recipe = recipe;
	servo->servo_wait_cycles = 0;
	servo->servo_start_loop = 0;
	servo->servo_additional_loops = 0;
	servo->recipe_exec = 0; // changed
	servo->servo_position = 0;
	servo->servo_lcv =  0;
	servo->servo_wait_time = 0;
	servo->servo_status = Status_Paused;// Changed
	servo->servo_state = State_At_Position; // Changed
	servo->servo_events = User_Paused_Recipe;
	
}


