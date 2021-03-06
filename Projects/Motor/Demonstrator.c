// Demonstrator.c Motor Speed Control version

#include "PC_Comm.h"
#include "Demonstrator.h"

unsigned char milliseconds = 0;
unsigned int second = 0; // count to 1000 and trigger one second event
unsigned int speed = 0; // IR detector count per second
unsigned int lastspeed = 0; // IR detector count per second

void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   
	
	// Initialize the USART
	USARTinit();

	// Set for pin change on PINB0
	DDRB = (1 << PINB0); // set pin 0 to output
	PORTB = (1 << PINB0); // set pin 0 to enable pullup
	PCMSK0 = (1 << PINB0); // 
	EIFR = (1 << 7); // flag for PCINT15-8
	EIMSK = (1 << 7); // mask for PCINT15-8

	DDRB = 0X00; // set PORTB for input	
    PORTB = 0xFF; // enable pullup on for input

	// set PORTD for output
	DDRD = (1 << PIND0); // set pin 0 to output
	PORTD = (1 << PIND0); // set pin 0 to enable pullup

	milliSecInit(127); // 50% duty cycle 1kHz signal

	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("You are talking to the Motor Speed Control demo.\r");
	sendString("setxxx to set speed\r");

}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 's':
			if( (s[1] == 'e') && (s[2] == 't'))
			parse_set(s);
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendString("You are talking to the Motor Speed Control demo.\r");
			break;
		default:
			sendString("\rYou sent: '");
			sendChar(s[0]);
			sendString("' - I don't understand.\r");
			break;
		
	}
	s[0] = '\0';
}

int parse_set(char s[])
{
	char set[11];
	unsigned char i = 3, j = 0;

	while( (s[i] != '\0') && (j <= 11) ) 
	{
	
		if( (s[i] >= '0') && (s[i] <= '9') )
		{
			set[j++] = s[i++];
		}
		else
		{
			sendString("Error - Parse_set received a non integer: ");
			sendChar(s[i]);
			sendChar('\r');
			return 0;
		}
	}
	
	set[j] = '\0';
	
	if(j>4)// must be < 256
	{
		sendString("Error - Parse_set number too large");
		return 0;
	}
	else
	{
		set_speed(atoi(set));
	}
		
	return 1;
}

void set_speed(int count)
{
	char speed[11];

	sendString("Setting the Compare Timer Count to: ");
	itoa(count,speed,10);	
	sendString(speed);
	sendChar('\r');
	
	milliSecInit(count);

}


/*
The USART init set the system oscillator to 2 mHz. We set the Timer0 prescaler
to clk/8 which gives a 250 kHz input to the timer/counter. A compare of 250 throws
an interrupt every millisecond.
*/
void milliSecInit(unsigned char count)
{
    // Enable timer0 compare interrupt
	TIMSK0 = (1<<OCIE0A);

	// Sets the compare value
	setOCR0A(count);
	
	// Set PWM Phase Correct mode, CLK/8 prescaler
	TCCR0A = (0<<FOC0A)|(0<<WGM01)|(1<<WGM00)|(1<<CS01);

}

void setOCR0A(unsigned char count)
{    
	// Sets the compare value
	OCR0A = count;
}


// Interrupt occurs twice per Millisec, timed for PWM
SIGNAL(SIG_OUTPUT_COMPARE0)
{
	// Toggle PORtD pin 0
	if(PORTD &= 1) cbi(PORTD, 0);
	else sbi(PORTD, 0);
}