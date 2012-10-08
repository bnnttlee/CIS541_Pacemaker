// Demonstrator.c Speedometer version

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
	
    // Init port pins
	DDRB |= 0x08; 
    PORTB |= ((1<<PINB4));//|(1<<PINB6)|(1<<PINB7));

    // Enable pin change interrupt on PORTB
	PCMSK1 = ((1<<PINB4));//|(1<<PINB6)|(1<<PINB7));
	EIFR = (1<<6)|(1<<7);
	EIMSK = (1<<6)|(1<<7);
	
	DDRD = 0xFF; // set PORTD for output
  	PORTD = 0XFF; // set LEDs off

	milliSecInit(127); // 50% duty cycle 1kHz signal
	
	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("You are talking to the Speedometer demo.\r");
	sendString("'setxxx' to set speed\r'Hz' to get speed in Hertz\r");

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
		case 'H':
			if( (s[1] == 'z'))
			sendSpeed();
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendString("You are talking to the Speedometer demo.\r");
			break;
		default:
			sendString("\rYou sent: '");
			sendChar(s[0]);
			sendString("' - I don't understand.\r");
			break;
		
	}
	s[0] = '\0';
}

void sendSpeed()
{
	char spd[11];
	
	sendString("Speed =  ");
	itoa(lastspeed,spd,10);	
	sendString(spd);
	sendChar('\r');	
	
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
		sendString("Error - Parse_set number too large\r");
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
	
	// get the speed count once per second
	if(second++ >= 1000)
	{
		second = 0;
		lastspeed = speed; // store most recent speed in Hz		
		speed = 0;
	}
}

SIGNAL(SIG_PIN_CHANGE1)
{
	speed++;
}