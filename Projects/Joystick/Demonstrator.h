// Demonstrator.h Joystick version

#include <avr/signal.h>
#include <inttypes.h>

#define KEY_UP      0
#define KEY_DOWN    1
#define KEY_LEFT    2
#define KEY_RIGHT   3
#define KEY_PUSH    4
#define KEY_INVALID 5

#define BUTTON_A    6   // UP
#define BUTTON_B    7   // DOWN
#define BUTTON_C    2   // LEFT
#define BUTTON_D    3   // RIGHT
#define BUTTON_O    4   // PUSH

#define PINB_MASK ((1<<PINB4)|(1<<PINB6)|(1<<PINB7))
#define PINE_MASK ((1<<PINE2)|(1<<PINE3))

#define TRUE 1
#define FALSE 0

// declare functions
void PinChangeInterrupt(void);
char getkey(void);

void initializer(void);
void parseInput(char *);
void joystick(void);