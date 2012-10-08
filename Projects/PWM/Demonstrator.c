// Demonstrator.c PWM version

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
	
	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("You are talking to the PWM demo.\r");
	
	pwmInit(127); // 50% duty cycle
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
			sendString("You are talking to the PWM demo.\r");
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
	
	pwmInit(count);

}

/*
The USART init set the system oscillator to 2 mHz. We set the Timer0 prescaler
to clk/8 which gives a 250 kHz input to the timer/counter. A compare of 250 throws
an interrupt every millisecond.
*/
void pwmInit(unsigned char count)
{
	// Initialize Timer0.

    // Enable timer0 compare interrupt
	TIMSK0 = (1<<OCIE0A);

	// Sets the compare value
	setOCR0A(count);
	
	// Set phase correct PWM, toggle OC0A, CLK/8 prescaler
	//TCCR0A = (0<<FOC0A)|(0<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(1<<COM0A0)|(1<<CS01);
	// The above causes the USART communication to cease. The toggling of the OC0A could
	// be the problem or it could be something else. TODO LATER - figure this out.
	
	// Set PWM Phase Correct mode, CLK/8 prescaler
	TCCR0A = (0<<FOC0A)|(0<<WGM01)|(1<<WGM00)|(1<<CS01);
}

void setOCR0A(unsigned char count)
{    
	// Sets the compare value
	OCR0A = count;
}

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	if(PORTD &= 1) cbi(PORTD, 0);
	else sbi(PORTD, 0);

}
