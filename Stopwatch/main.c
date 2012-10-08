#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <inttypes.h>

#include "main.h"
#include "lcd_functions.h"
#include "lcd_driver.h"
#include "bcd.h"

#define pLCDREG_test (*(char *)(0xEC))

volatile uint8_t gMILLISECOND;
volatile uint8_t gSECOND;
volatile uint8_t gMINUTE;
// mt static char __flash *statetext;
//(PGM_P const char*)

PGM_P time;
	
int main(){ 

	int i;
	// Calibrate the oscillator:
   // OSCCAL_calibration();   

	// Initialize the USART
	//USARTinit();
	
	sei();					// enable global interrupt
	
	// Initialize the RTC
	RTC_init();
	// Program initalization
    LCD_Init();
		
	while(1){
	i = i+1;
	showClock();
	}
	
	return 0;
}


void showClock(void)
{
	uint8_t MH, ML, SH, SL, MSH, MSL;

    MH= CHAR2BCD2(gMINUTE);   
        
    ML = (MH & 0x0F) + '0';
    MH = (MH >> 4) + '0';

    SH = CHAR2BCD2(gSECOND);
    SL = (SH & 0x0F) + '0';
    SH = (SH >> 4) + '0';

    MSH = CHAR2BCD2(gMILLISECOND);
    MSL = (MSH & 0x0F) + '0';
    MSH = (MSH >> 4) + '0';
    
    LCD_putc(0, MH);
    LCD_putc(1, ML);
    LCD_putc(2, SH);
    LCD_putc(3, SL);
    LCD_putc(4, MSH);
    LCD_putc(5, MSL);
    LCD_putc(6, '\0');
    
    LCD_Colon(1);
    
    LCD_UpdateRequired(TRUE, 0);
}


void RTC_init(void)
{ 
	//Wait for 1 sec
	//delay(1000);
	
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

    // initial time and date setting
    gMILLISECOND  = 0;
    gSECOND  = 0;
    gMINUTE    = 0;
}


SIGNAL(SIG_OUTPUT_COMPARE0)
{
    gMILLISECOND++;		// increment second

    if (gMILLISECOND == 99)
    {
        gMILLISECOND = 0;
        gSECOND++;		// increment minute
        
        if (gSECOND > 59)
        {
            gSECOND = 0;
            gMINUTE++;	// increment hour
            
            if (gMINUTE > 59)
            {             
                gMINUTE = 0;
			}
        }
    }
}

