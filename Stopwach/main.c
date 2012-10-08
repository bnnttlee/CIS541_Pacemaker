#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <inttypes.h>

#include "main.h"
#include "lcd_functions.h"
#include "lcd_driver.h"

#define pLCDREG_test (*(char *)(0xEC))

unsigned char gMILLISECOND;
unsigned char gSECOND;
unsigned char gMINUTE;
// mt static char __flash *statetext;
//(PGM_P const char*)
PGM_P time;
	
void initializer()
{
	// Calibrate the oscillator:
   // OSCCAL_calibration();   

	// Initialize the USART
	//USARTinit();
	
	sei();					// enable global interrupt
	
	// Initialize the RTC
	RTC_init();
	// Program initalization
    LCD_Init();
		
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
 
}

void update_LCD(void)
{    
	
	// Initial state variables
	time = PSTR("HHMMSS");

	for (;;)            // Main loop
    {
		if (time)
        {
			LCD_puts_f(time, 0);
			LCD_Colon(1);
			time = NULL;   
		}
    } //End Main loop
	
	return 0;
}
// convert a character into a binary coded decimal chracter in the range 0 to 99
// resulting byte has tens in high nibble and ones in low nibble
char CHAR2BCD2(char input)
{
    char high = 0;
    
    while (input >= 10)                 // Count tens
    {
        high++;
        input -= 10;
    }

    return  (high << 4) | input;        // Add ones and return answer
}



// initialize Timer/counter2 as asynchronous using the 32.768kHz watch crystal.
void RTC_init(void)
{ 
	
	/set Clock Prescaler Change Enable
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

