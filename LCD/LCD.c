#include "LCD_driver.h"
#include "LCD_functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

int main(){

	
	int i = 0;

	/******************
		PORTD init
	*******************/
	
	//Set direction to [OUTPUT]
	DDRD = 0xFF;
	
	//set the PORTD0 to high(1)
	PORTD = 0x01;
	
	//Do Nothing...
	
	const char LCD_START_msg = 'LCD demo';
	LCD_Init();
	LCD_puts_f(LCD_START_msg, 1);
	while(1){
	i = i+1;
	 PORTD = 0x01;
	}
	
	return 0;

}