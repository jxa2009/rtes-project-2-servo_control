#ifndef RECIPES_H
#define RECIPES_H

// The bits that correspond to the opcode
#define OPCODE    (0xE0)
//The bits that correspond to the parameter
#define PARAMETER (0x1F)

// Different recipe commands (opcodes)
#define MOV         (0x20)
#define WAIT        (0x40)
#define LOOP        (0x80)
#define END_LOOP    (0xA0)
#define END_RECIPE   (0x00)

// Pulse width time for positions (in ms) (steps of .32ms)
// (400 equates to 0.4 ms)
#define POSITION_0 (2000)
#define POSITION_1 (1680)//(16.8)
#define POSITION_2 (1360)// (13.6)
#define POSITION_3 (1040)
#define POSITION_4 (720)
#define POSITION_5 (400)

// Recipes are statically compiled into the program
unsigned char recipe1[] = {};
unsigned char recipe2[] = {};


// Pointer to the recipes for ease of access
unsigned char *recipes[] = {recipe1, recipe2, NULL};

// Global State value
// Initial State as paused
static Servo_StateE current_servo_state = Status_Paused;
// Enum for statuses, that will be used to glow up the LEDs
typedef enum Status_E
{
    Status_Running,
    Status_Paused,
    Status_Command_Error,
    Status_Nested_Error
} StatusE;

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
    User_Begin_Restart_Recipe,
    Recipe_Ended
} EventsE;

int move_servo_to_position( uint32_t *servo,int position);
void recipe_parse( uint32_t *servo, unsigned char *recipe);
#endif