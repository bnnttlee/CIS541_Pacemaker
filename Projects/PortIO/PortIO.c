// PortIO.c
#include <avr/io.h>

int main (void)
{
    // Init port pins
	DDRB = 0x00; // set port B for input
 	DDRD = 0xFF; // set port D for output	

	while(1)
	{
		PORTD = PINB;
	}
}
		
		

	