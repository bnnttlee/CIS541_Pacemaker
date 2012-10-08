// CylonEyes.c

#include <avr/io.h>
#include <avr/delay.h>

int main (void)
{
	// declare and initialize the scroll delay_count
	unsigned long delay_count = 10000;
	
	// declare a variable for the speed increase
	unsigned long increase = 0;
	
	// declare a variable for the polarity
	unsigned char polarity = 0;
	
	
	// Init port pins
	DDRB = 0x00; // set port B for input
	DDRD = 0xFF; // set port D for output	

	
	while(1)
	{
		// read the switches
		increase = PINB;
		
		// set the polarity
		if(increase > 127) 
		{
			increase -= 127;
			polarity = 1;
		}
		else polarity = 0;
		
		// set the delay count
		delay_count = 5000 + (increase * 500);
		
		// scroll those eyes
		for(int i = 1; i <= 128; i = i*2)
		{
			if(polarity) PORTD = ~i;
			else PORTD = i;
			_delay_loop_2(delay_count);
		}	
			
		for(int i = 128; i > 1; i -= i/2)
		{
			if(polarity) PORTD = ~i;
			else PORTD = i;
			_delay_loop_2(delay_count);
		}	
		
	}
}

