// Demonstrator.c Joystick version

#include "PC_Comm.h"
#include "Demonstrator.h"

// declare global variables
volatile char KEY = 0;
volatile char KEY_VALID = 0;
volatile char ENABLED = 1; // start enabled

void initializer()
{

	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
    // Init port pins
	DDRB |= 0xD8;
    PORTB |= PINB_MASK;
    DDRE = 0x00;
    PORTE |= PINE_MASK;

    // Enable pin change interrupt on PORTB and PORTE
	PCMSK0 = PINE_MASK;
	PCMSK1 = PINB_MASK;
	EIFR = (1<<6)|(1<<7);
	EIMSK = (1<<6)|(1<<7);
	
	DDRD = 0xFF; // set PORTD for output
	DDRB = 0X00; // set PORTB for input
	
    PORTB = 0xFF; // enable pullup on for input
   	PORTD = 0XFF; // set LEDs off
	
	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("You are talking to the JoyStick demo.\r");

}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 'j':
			if( (s[1] == 'o') && (s[2] == 'y'))
			joystick();
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendString("You are talking to the LightMeter demo.\r");
			break;
		default:
			sendString("\rYou sent: '");
			sendChar(s[0]);
			sendString("' - I don't understand.\r");
			break;
		
	}
	s[0] = '\0';
}

void joystick()
{
	if(ENABLED == 0) ENABLED = 1;
	else ENABLED = 0;
}



SIGNAL(SIG_PIN_CHANGE0)
{
    PinChangeInterrupt();
}

SIGNAL(SIG_PIN_CHANGE1)
{
    PinChangeInterrupt();    
}

void PinChangeInterrupt(void)
{
    char buttons;
    char key;

    buttons = (~PINB) & PINB_MASK;
    buttons |= (~PINE) & PINE_MASK;

    // Output virtual keys
    if (buttons & (1<<BUTTON_A))
        key = KEY_UP;
    else if (buttons & (1<<BUTTON_B))
        key = KEY_DOWN;
    else if (buttons & (1<<BUTTON_C))
        key = KEY_LEFT;
    else if (buttons & (1<<BUTTON_D))
        key = KEY_RIGHT;
    else if (buttons & (1<<BUTTON_O))
        key = KEY_PUSH;
    else
        key = KEY_INVALID;
  
    if(key != KEY_INVALID)
    {
        if (!KEY_VALID)
        {
            KEY = key;          // Store key in global key buffer
            KEY_VALID = TRUE;
        }
    }
    
    EIFR = (1<<PCIF1) | (1<<PCIF0);     // Delete pin change interrupt flags 
	
	if(ENABLED)
	{
		getkey();
	}
}



char getkey(void)
{
    char k;

    cli(); // disable interrrupts so 'KEY' won't change while in use

    if (KEY_VALID) // Check for unread key in buffer
    {
        k = KEY;
        KEY_VALID = FALSE;
    }
    else
        k = KEY_INVALID; // No key stroke available

    sei(); // enable interrupts

	if(k != KEY_INVALID)
	{
		sendString("The joystick position is: ");
		
		switch(k)
		{
			case KEY_UP:
				sendString("UP");
				break;
			case KEY_DOWN:
				sendString("DOWN");
				break;
			case KEY_LEFT:
				sendString("LEFT");
				break;
			case KEY_RIGHT:
				sendString("RIGHT");
				break;
			case KEY_PUSH:
				sendString("PUSH");
				break;
			default:
				sendString("?");
				break;
		}
		
		sendChar('\r');
	}	

    return k;
}