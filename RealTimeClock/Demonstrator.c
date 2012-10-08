// Demonstrator.c Real Time Clock version

#include "PC_Comm.h"
#include "Messages.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

unsigned char gSECOND;
unsigned char gMINUTE;
unsigned char gHOUR;

void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
	// Initialize the RTC
	RTC_init();
		
}


void showClock(void)
{
	uint8_t HH, HL, MH, ML, SH, SL;

    HH = CHAR2BCD2(gHOUR);   
        
    HL = (HH & 0x0F) + '0';
    HH = (HH >> 4) + '0';

    MH = CHAR2BCD2(gMINUTE);
    ML = (MH & 0x0F) + '0';
    MH = (MH >> 4) + '0';

    SH = CHAR2BCD2(gSECOND);
    SL = (SH & 0x0F) + '0';
    SH = (SH >> 4) + '0';
 
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

	
	//Now Timer will run 26315Hz/256 = 102Hz => .01 s (10 ms)

     // wait for external crystal to stabilise
	for(int i = 0; i < 10; i++)
			_delay_loop_2(30000);   
			
    cli();					// disabel global interrupt
	
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

	
	// wait for TCN2UB and TCR2UB to be cleared
    while((ASSR & 0x01) | (ASSR & 0x04));	

    TIFR2 = 0xFF;			// clear interrupt-flags
    sbi(TIMSK2, TOIE2);		// enable Timer2 overflow interrupt

    sei();					// enable global interrupt

    // initial time and date setting
    gMILLISECOND  = 0;
    gSECOND  = 0;
    gMINUTE    = 0;
}

// one second interrupt from 32kHz watch crystal
SIGNAL(SIG_OVERFLOW2)
{
    gSECOND++;		// increment second

    if (gSECOND == 60)
    {
        gSECOND = 0;
        gMINUTE++;		// increment minute
        
        if (gMINUTE > 59)
        {
            gMINUTE = 0;
            gHOUR++;	// increment hour
            
            if (gHOUR > 12)
            {             
                gHOUR = 0;
			}
        }
    }
}

