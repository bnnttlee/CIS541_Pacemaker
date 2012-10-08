//Blinky.c

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

int TIMER_COUNT = 0;
int LED_STATE = 0;
void Timer0_Init(void);

int main(){
	
	int i = 0;
	
	/******************
	Enable Interrupt
	********************/
	
	sei();
	
	/******************
		PORTD init
	*******************/
	
	//Set direction to [OUTPUT]
	DDRD = 0xFF;
	
	//set the PORTD0 to high(1)
	PORTD = 0x01;
	
	/*******************
	Initialization of TIMER0
	*******************/
	Timer0_Init();
	
	//Do Nothing...
	
	while(1){
	i = i+1;
	}
	
	return 0;
}

void Timer0_Init(void)
{
	//set Clock Prescaler Change Enable
	CLKPR = (1<<CLKPCE);
	
	//set prescaler = 8, Inter RC 8MHz/8 = 1MHz
	CLKPR = (1<<CLKPS1) | (1<<CLKPS0);
	
	//Enable timer0 compare interrupt
	TIMSK0 = (1<<OCIE0A);
	
	//Sets the compare value
	OCR0A = 38;
	
	//Now, Timer will run 1Mhz/38 = 26315 Hz
	
	//Set Clear on Timer Compare (CTC) mode, CLK/256 prescaler
	TCCR0A = (1<<WGM01) | (0<<WGM00) | (4<<CS00);
	
	//Now Timer will run 26315Hz/256 = 102Hz => .01 s (10 ms)
	
}

/**************************************
	Interrupt Serve Routine of Timer 0
	(Clear on Timer Compare (CTC) Mode)
***********************************/

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	uint8_t i;
	TIMER_COUNT++;
	if (TIMER_COUNT == 100){
		if (LED_STATE == 0){
			LED_STATE = 1;
			PORTD = 0x01;
		}
		else{
		LED_STATE = 0;
		PORTD = 0x00;
		}
		TIMER_COUNT = 0;
	}
}
	