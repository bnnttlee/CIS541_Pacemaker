#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

int TIMER_COUNT = 0;
int LED_STATE = 0;
void TIMER0_INIT(void);

int main(){
	int i = 0;

	//Enable interrupt
	sei();
	//Set direction to output
	DDRD = 0xFF;
	//Set portD0 to high
	PORTD = 0x01;
	//Initialization of Timer 0
	Timer0_Init();
	while(1)
	//Do nothing
	{
	i = i+1;
	}
	return 0;
} //end main




void Timer0_Init(void)
{
	//set Clock prescaler Change Enable
	CLKPR = (1<<CLKPCE);
	//set prescaler = 8, Inter RC 8 MHz / 8 = 1 MHz
	CLKPR = (1<<CLKPS1) | (1<<CLKPS0);
	//Enable timer0 compare interrupt
	TIMSK0 = (1<<OCIE0A);
	//sets compare value 
	OCR0A = 38;

	// Now timer runs at 1 MHz/ 38 = 26315 Hz

	//Set Clear on Timer Compare (CTC) mode, CLK/256 prescaler
	TCCR0A = (1<WGM01)|(0<<WGM00)|(4<<CS00);

	//Now timer runs at 26315/256 = 102 Hz => 0.01 s (10 ms)

}// end TIMERINIT

//****************************
//Interrupt Service Routine of timer0
//Clear on Timer Compare (CTC) mode
//**********************************

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	uint8_t i;
	TIMER_COUNT++;
	if(TIMER_COUNT ==1)
	{
		if(LED_STATE == 0){
		LED_STATE = 1;
		PORTD = 0x01;
		}
		else{
		LED_STATE = 0;
		PORTD = 0x00;
		}
		TIMER_COUNT = 0;
	}
}//end SIGNAL 
