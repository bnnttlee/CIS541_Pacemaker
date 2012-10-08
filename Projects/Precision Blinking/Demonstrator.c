// Demonstrator.c Precision Blinking version

#include "PC_Comm.h"
#include "Demonstrator.h"

unsigned char milliseconds = 0;

void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
	// set PORTD for output
	DDRD = 0xFF;
	
	MilliSec_init(250); // default to 1000 Hz

	
	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("You are talking to the Precision Blinking demo.\r");

}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 'c':
			if( (s[1] == 't') && (s[2] == 'c'))
			parse_ctc(s);
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendString("You are talking to the Precision Blinking demo.\r");
			break;
		default:
			sendString("\rYou sent: '");
			sendChar(s[0]);
			sendString("' - I don't understand.\r");
			break;
		
	}
	s[0] = '\0';
}

int parse_ctc(char s[])
{
	char ctc[11];
	unsigned char i = 3, j = 0;

	while( (s[i] != '\0') && (j <= 11) ) 
	{
	
		if( (s[i] >= '0') && (s[i] <= '9') )
		{
			ctc[j++] = s[i++];
		}
		else
		{
			sendString("Error - Parse_ctc received a non integer: ");
			sendChar(s[i]);
			sendChar('\r');
			return 0;
		}
	}
	
	ctc[j] = '\0';
	
	if(j>4)// must be < 256
	{
		sendString("Error - Parse_ctc number too large");
		return 0;
	}
	else
	{
		set_ctc(atoi(ctc));
	}
		
	return 1;
}

void set_ctc(int count)
{
	char ctc[11];

	sendString("Setting the Compare Timer Count to: ");
	itoa(count,ctc,10);	
	sendString(ctc);
	sendChar('\r');
	
	MilliSec_init(count);

}

/*
The USART init set the system oscillator to 2 mHz. We set the Timer0 prescaler
to clk/8 which gives a 250 kHz input to the timer/counter. A compare of 250 throws
an interrupt every millisecond.
*/
void MilliSec_init(unsigned char count)
{
	// Initialize Timer0.

    // Enable timer0 compare interrupt
	TIMSK0 = (1<<OCIE0A);

	// Sets the compare value
	set_OCR0A(count);
	
	// Set Clear on Timer Compare (CTC) mode, CLK/8 prescaler
	TCCR0A = (1<<WGM01)|(0<<WGM00)|(1<<CS01);

}

void set_OCR0A(unsigned char count)
{    
	// Sets the compare value
	OCR0A = count;
}


// Interrupt occurs once per millisecond
SIGNAL(SIG_OUTPUT_COMPARE0)
{
	PORTD = milliseconds++;
}
