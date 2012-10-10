#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <avr/signal.h>
#include <avr/sleep.h>
#include <inttypes.h>

#include "main.h"
#include "lcd_functions.h"
#include "lcd_driver.h"
#include "bcd.h"

#define pLCDREG_test (*(char *)(0xEC))

/*************************************
 Setting up joystick
 *************************************/

#define KEY_NULL    0
#define KEY_START    1
#define KEY_PAUSE    2
#define KEY_RESET    3

#define BUTTON_A    6   // UP
#define BUTTON_B    7   // DOWN
#define BUTTON_C    2   // LEFT
#define BUTTON_D    3   // RIGHT
#define BUTTON_O    4   // PUSH

#define FALSE   0
#define TRUE    (!FALSE)
#define NULL    0

#define PINB_MASK ((1<<PINB4)|(1<<PINB6)|(1<<PINB7))
#define PINE_MASK ((1<<PINE2)|(1<<PINE3))

// declare functions
void PinChangeInterrupt(void);
char getkey(void);

// declare global variables
volatile char KEY = NULL;
volatile char KEY_VALID = FALSE;
volatile char active = FALSE;

volatile uint8_t gMILLISECOND;
volatile uint8_t gSECOND;
volatile uint8_t gMINUTE;
char input;
// mt static char __flash *statetext;
//(PGM_P const char*)

PGM_P time;
	
int main(){ 

	int i;
	// Calibrate the oscillator:
	OSCCAL_calibration();   

	// Initialize the USART
	//USARTinit();
    
    // Init port pins
	DDRB |= 0xD8;
    PORTB |= PINB_MASK;
    DDRE = 0x00;
    PORTE |= PINE_MASK;
    
    // Enable pin change interrupt on PORTB and PORTE
	PCMSK0 = PINE_MASK;
	PCMSK1 = PINB_MASK;
	EIFR = (1<<6)|(1<<7);
	EIMSK = (1<<6)|(1<<7);
    
    DDRD = 0xFF; // set PORTD for output
	DDRB = 0X00; // set PORTB for input
	
    PORTB = 0xFF; // enable pullup on for input
   	PORTD = 0XFF; // set LEDs off
	
	sei();
    // enable global interrupt
	
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
    if (active)
    {
    gMILLISECOND++;// increment second
    }

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

//Calibrate the internal OSCCAL byte, using the external
//32,768 kHz crystal as reference
void OSCCAL_calibration(void)
{
    unsigned char calibrate = 0;//FALSE;
    int temp;
    unsigned char tempL;
    
    CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
    // set prescaler = 8, Inter RC 8Mhz / 8 = 1Mhz
    CLKPR = (1<<CLKPS1) | (1<<CLKPS0);
    
    TIMSK2 = 0;             //disable OCIE2A and TOIE2
    
    ASSR = (1<<AS2);        //select asynchronous operation of timer2 (32,768kHz)
    
    OCR2A = 200;            // set timer2 compare value
    
    TIMSK0 = 0;             // delete any interrupt sources
    
    TCCR1B = (1<<CS10);     // start timer1 with no prescaling
    TCCR2A = (1<<CS20);     // start timer2 with no prescaling
    
    while((ASSR & 0x01) | (ASSR & 0x04));       //wait for TCN2UB and TCR2UB to be cleared
    
    // wait for external crystal to stabilise
	for(int i = 0; i < 10; i++)
        _delay_loop_2(30000);
    
    while(!calibrate)
    {
        cli(); // mt __disable_interrupt();  // disable global interrupt
        
        TIFR1 = 0xFF;   // delete TIFR1 flags
        TIFR2 = 0xFF;   // delete TIFR2 flags
        
        TCNT1H = 0;     // clear timer1 counter
        TCNT1L = 0;
        TCNT2 = 0;      // clear timer2 counter
        
        while ( !(TIFR2 && (1<<OCF2A)) );   // wait for timer2 compareflag
        
        TCCR1B = 0; // stop timer1
        
        sei(); // __enable_interrupt();  // enable global interrupt
        
        if ( (TIFR1 && (1<<TOV1)) )
        {
            temp = 0xFFFF;      // if timer1 overflows, set the temp to 0xFFFF
        }
        else
        {   // read out the timer1 counter value
            tempL = TCNT1L;
            temp = TCNT1H;
            temp = (temp << 8);
            temp += tempL;
        }
        
        if (temp > 6250)
        {
            OSCCAL--;   // the internRC oscillator runs to fast, decrease the OSCCAL
        }
        else if (temp < 6120)
        {
            OSCCAL++;   // the internRC oscillator runs to slow, increase the OSCCAL
        }
        else
            calibrate = 1;//TRUE;   // the interRC is correct
        
        TCCR1B = (1<<CS10); // start timer1
    }
}

SIGNAL(SIG_PIN_CHANGE0)
{
    PinChangeInterrupt();
}

SIGNAL(SIG_PIN_CHANGE1)
{
    PinChangeInterrupt();
}

void PinChangeInterrupt(void)
{
    char buttons;
    char key;
    
    buttons = (~PINB) & PINB_MASK;
    buttons |= (~PINE) & PINE_MASK;
    
    // Output virtual keys
    if (buttons & (1<<BUTTON_B))
	{
        key = KEY_PAUSE;
        active = FALSE;
		
	}
    else if (buttons & (1<<BUTTON_D))
	{
        key = KEY_RESET;
        // reset timer
        gMILLISECOND  = 0;
        gSECOND = 0;
        gMINUTE = 0;
	}
    else if (buttons & (1<<BUTTON_O))
     {   key = KEY_START;
        active = TRUE;
	}
    else
    {    key = KEY_NULL;
	}
    
    if(key != KEY_NULL)
    {
        if (!KEY_VALID)
        {
            KEY = key;          // Store key in global key buffer
            KEY_VALID = TRUE;
            //getkey();
        }
    }
    
    EIFR = (1<<PCIF1) | (1<<PCIF0);     // Delete pin change interrupt flags
}



char getkey(void)
{
    char k;
    
    cli(); // disable interrrupts so 'KEY' won't change while in use
    
    if (KEY_VALID) // Check for unread key in buffer
    {
        k = KEY;
        KEY_VALID = FALSE;
    }
    else
        k = KEY_NULL; // No key stroke available
    
    sei(); // enable interrupts
    
    return k;
}


