#ifndef RECIPES_H
#define RECIPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// The bits that correspond to the opcode
#define OPCODE    (0xE0)
//The bits that correspond to the parameter
#define PARAMETER (0x1F)

// Different recipe commands (opcodes)
#define MOV         (0x20)
#define WAIT        (0x40)
#define LOOP        (0x80)
#define END_LOOP    (0xA0)
#define END_RECIPE  (0x00)
#define SHIFT       (0xE0)
#define EDGE        (0x60)

// Pulse width time for positions (in ms) (steps of .32ms)
// (400 equates to 0.4 ms)
#define POSITION_0 (500)
#define POSITION_1 (720)
#define POSITION_2 (1040)
#define POSITION_3 (1360)
#define POSITION_4 (1680)
#define POSITION_5 (2000)




// Statically defined variables

//static int servo_1_wait_cycles = 0;
//static int servo_1_start_loop = 0;
//static int servo_1_additional_loops = 0;
//		
//static int recipe_1_exec = 1;
//static int recipe_2_exec = 0;

//static int servo_1_position = 0;
//static int servo_2_position = 5;

//static int servo_1_lcv = 0;
//static int servo_2_lcv = 0;

//static int servo_1_wait_time = 0;
//static int servo_2_wait_time = 0;

// Recipes are statically compiled into the program
//static unsigned char recipe1[] = {LOOP+2,MOV+0,MOV+1,MOV+2,SHIFT+4,MOV+3,MOV+4,MOV+5,END_LOOP,MOV+0,END_RECIPE};
static unsigned char recipe1[] = {LOOP+3,SHIFT+4,END_LOOP,END_RECIPE};
static unsigned char recipe2[] = {LOOP+3,SHIFT+4,END_LOOP,END_RECIPE};
//static unsigned char recipe1[]  = {MOV+0,EDGE+1,EDGE+0,WAIT+10,MOV+5,MOV+3,MOV+2,LOOP+1,LOOP+1,MOV+1,MOV+4,END_LOOP,MOV+0,MOV+2,WAIT+0,MOV+3,WAIT+0,MOV+2,MOV+3,WAIT+31,MOV+4,MOV+0,END_RECIPE,MOV+4,MOV+0,END_RECIPE};
//static unsigned char recipe2[]  = {MOV+0,MOV+5,MOV+5,MOV+7,LOOP+0,MOV+1,MOV+4,END_LOOP,MOV+0,MOV+2,WAIT+0,MOV+3,WAIT+0,MOV+2,MOV+3,WAIT+31,MOV+4,MOV+0,END_RECIPE,MOV+4,MOV+0,END_RECIPE};
//static unsigned char recipe1[] = {MOV+0,MOV+5,MOV+0,MOV+3,LOOP+0,MOV+1,MOV+4,END_LOOP,MOV+0,MOV+2,WAIT+0,MOV+3,WAIT+0,MOV+2,MOV+3,WAIT+31,WAIT+31,WAIT+31,MOV+4,END_RECIPE};
//static unsigned char recipe1[] = {LOOP+2,MOV+0,MOV+1,MOV+2,SHIFT+4,MOV+3,MOV+4,MOV+5,END_LOOP,MOV+0,END_RECIPE};
//static unsigned char recipe2[] = {LOOP+2,MOV+0,MOV+1,MOV+2,SHIFT+4,MOV+3,MOV+4,MOV+5,END_LOOP,MOV+0,END_RECIPE};

// Enum for statuses, that will be used to glow up the LEDs
typedef enum Status_E
{
    Status_Running = 1,
    Status_Paused = 0,
    Status_Command_Error = 2,
    Status_Nested_Error = 3
} StatusE;

// Enum for the state of individual servos
typedef enum Servo_State_E
{
    State_At_Position,
    State_Unknown,
    State_Moving,
    State_Recipe_Ended
} Servo_StateE;

//Event Transistions between states
typedef enum Events_E
{
    User_Paused_Recipe,
    User_Continued_Recipe,
    User_Entered_Left,
    User_Entered_Right,
		User_Entered_No_Op,
    User_Begin_Restart_Recipe,
		User_Entered_Invalid_Recipe,
    Recipe_Ended
} EventsE;

// Struct that holds all the data of a servo that is necessary
typedef struct Servo_S
{
	uint32_t* servo_ccr;
	unsigned char* servo_recipe;
	int servo_wait_cycles;
	int servo_start_loop;
	int servo_additional_loops;
	int recipe_exec;
	int servo_position;
	int servo_lcv;
	int servo_wait_time;
	StatusE servo_status;
	Servo_StateE servo_state;
	EventsE servo_events;
} ServoS;
// Create two global servos

static ServoS servo_1;
static ServoS servo_2;

void init_servo(ServoS* servo, unsigned char* recipe, uint32_t* timer);
int move_servo_to_position( uint32_t *servo, int position);
EventsE user_command_parse(char input);
void event_command_parse(EventsE generated_event, ServoS* servo);
#endif
