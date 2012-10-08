// Blinky.c

#include <avr/io.h>
#include <avr/delay.h>

void wait(void);

int main (void)
{
	
	// set PORTD for output
	DDRD = 0xFF;
	
	while(1)
	{
	
		for(int i = 1; i <= 128; i = i*2)
		{
			PORTD = i;
			_delay_loop_2(30000);
		}	
			
		for(int i = 128; i > 1; i -= i/2)
		{
			PORTD = i;
			_delay_loop_2(30000);
		}	
		
	}
	
}

