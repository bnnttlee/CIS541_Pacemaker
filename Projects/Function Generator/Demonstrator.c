// Demonstrator.c Function Generator / Digital Oscilloscope version

#include "PC_Comm.h"
#include "Messages.h"
#include "WaveTables.h"

unsigned char count = 0;
unsigned char tenth = 0;
//unsigned long signal = 0; // used for test
	
void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
	// set PORTD for output
	DDRD = 0xFF;
	
	// Display instructions on PC
	sendFString(TALKING_TO);
	sendFString(WHO_DEMO);
	
	sendFString(ENTER);
	sendFString(TEXT_CTC);
	
	sendFString(ENTER);
	sendFString(TEXT_SINE);
	sendFString(TO_START);	
	sendFString(TEXT_SINE);
	sendFString(WAVE);

	sendFString(ENTER);
	sendFString(TEXT_SQUARE);
	sendFString(TO_START);	
	sendFString(TEXT_SQUARE);
	sendFString(WAVE);
	
	sendFString(ENTER);
	sendFString(TEXT_SAWTOOTH);
	sendFString(TO_START);	
	sendFString(TEXT_SAWTOOTH);
	sendFString(WAVE);
	
	sendFString(ENTER);
	sendFString(TEXT_TRIANGLE);
	sendFString(TO_START);	
	sendFString(TEXT_TRIANGLE);
	sendFString(WAVE);	
	
	
	MilliSec_init(250); // default to 1000 Hz	
	
	DigitalOscilloscopeTimerInit();

	ADC_init();
	
	startSine();
}

void parseInput(char s[])
{

	// parse first character	
	switch (s[0])
	{
		case 's':
			if( (s[1] == 'i') && (s[2] == 'n')&& (s[3] == 'e'))
				startSine();
			else if( (s[1] == 'q') && (s[2] == 'u')&& (s[3] == 'a')&& (s[4] == 'r')&& (s[5] == 'e'))
				startSquare();
			else if( (s[1] == 'a') && (s[2] == 'w')&& (s[3] == 't')&& (s[4] == 'o')&& (s[5] == 'o')&& (s[6] == 't')&& (s[7] == 'h'))
				startSawtooth();
			break;
		case 't':
			if( (s[1] == 'r') && (s[2] == 'i')&& (s[3] == 'a')&& (s[4] == 'n')&& (s[5] == 'g')&& (s[6] == 'l')&& (s[7] == 'e'))
			startTriangle();
			break;		
		case 'c':
			if( (s[1] == 't') && (s[2] == 'c'))
			parse_ctc(s);
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendFString(TALKING_TO);
			sendFString(WHO_DEMO);
			break;
		default:
			sendFString(BAD_COMMAND1);
			sendChar(s[0]);
			sendFString(BAD_COMMAND2);

			break;
		
	}
	s[0] = '\0';
}

int parse_ctc(char s[])
{
	char ctc[11];
	unsigned char i = 3, j = 0;

	while( (s[i] != '\0') && (j <= 11) ) 
	{
	
		if( (s[i] >= '0') && (s[i] <= '9') )
		{
			ctc[j++] = s[i++];
		}
		else
		{
			sendFString(ERROR_NONINT);
			sendChar(s[i]);
			sendChar('\r');
			return 0;
		}
	}
	
	ctc[j] = '\0';
	
	if(j>4)// must be < 256
	{
		sendFString(ERROR_NUMTOLARGE);
		return 0;
	}
	else
	{
		set_ctc(atoi(ctc));
	}
		
	return 1;
}

void set_ctc(int count)
{
	char ctc[11];

	sendString("Setting the Compare Timer Count to: ");
	itoa(count,ctc,10);	
	sendString(ctc);
	sendChar('\r');
	
	MilliSec_init(count);

}

void startWave(int wave)
{	
 	sendFString(TEXT_SETTING);
	sendFString(TEXT_WAVE_TBL[wave]);     // Send the song title to the PC
	sendChar('\r');
            
	pWave=(int*)pgm_read_word(&Waves[wave]); // looks too complicated..
}

void startSine()
{
	startWave(0);
}
void startSquare()
{
	startWave(1);
}
void startSawtooth()
{
	startWave(2);
}
void startTriangle()
{
	startWave(3);
}


/*
The USART init set the system oscillator to 2 mHz. We set the Timer0 prescaler
to clk/8 which gives a 250 kHz input to the timer/counter. A compare of 250 throws
an interrupt every millisecond.
*/
void MilliSec_init(unsigned char count)
{
	// Initialize Timer0.

    // Enable timer0 compare interrupt
	TIMSK0 = (1<<OCIE0A);

	// Sets the compare value
	set_OCR0A(count);
	
	// Set Clear on Timer Compare (CTC) mode, 
	TCCR0A = (1<<WGM01)|(0<<WGM00)|(1<<CS02)|(0<<CS01)|(0<<CS00);

}

// Initialize for 1 millisecond interrupt
void DigitalOscilloscopeTimerInit()
{
	// Initialize Timer2.

    // Enable timer2 compare interrupt
	TIMSK2 = (1<<OCIE2A);

	// Sets the compare value
	OCR2A = 1;
	
	// Set Clear on Timer Compare (CTC) mode, 
	TCCR2A = (1<<WGM21)|(0<<WGM20)|(1<<CS22)|(0<<CS21)|(0<<CS20);

}


void set_OCR0A(unsigned char count)
{    
	// Sets the compare value
	OCR0A = count;
}


// Interrupt occurs once per millisecond
SIGNAL(SIG_OUTPUT_COMPARE0)
{
//	signal += pgm_read_word(pWave + count);  // used for test
	PORTD = pgm_read_word(pWave + count++);      // read table
	tenth++;	
}


// Interrupt occurs once per millisecond
SIGNAL(SIG_OUTPUT_COMPARE2)
{
	int sig = 0;
	
	sig = ADC_read();
	
	if (tenth >= 10) 
	{
		tenth = 0;
		
		for(int i = 0; i < (sig/4); i++)
		{
			sendChar(' ');
		}
		sendChar('*');
		sendChar('\r');
	}	

/*	// Test code to output wave from table to Hyperterminal
	if (tenth >= 10) 
	{
		tenth = 0;
		signal /= 50;
		
		for(int i = 0; i < signal; i++)
		{
			sendChar(' ');
		}
		sendChar('*');
		sendChar('\r');
		signal = 0;
	}	
*/	

}

/**********************************************************************************
	ADC common functions
***********************************************************************************/

void ADC_init()
{
	int dummy = 0;
	
    ADMUX = 1;   

    // set ADC prescaler to , 1MHz / 8 = 125kHz    
    ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);    

    //  Take a dummy reading , which basically allows the ADC 
    // to hack up any hairballs before we take any real readings
    dummy = ADC_read();       
}

int ADC_read(void)
{
    char i;
    int ADC_temp;
	// mt int ADC = 0 ;
	int ADCr = 0;
    
    // To save power, the voltage over the LDR and the NTC is turned off when not used
    // This is done by controlling the voltage from a I/O-pin (PORTF3)
    sbi(PORTF, PF3); // mt sbi(PORTF, PORTF3);     // Enable the VCP (VC-peripheral)
    sbi(DDRF, DDF3); // sbi(DDRF, PORTF3);        

    sbi(ADCSRA, ADEN);     // Enable the ADC

    //do a dummy readout first
    ADCSRA |= (1<<ADSC);        // do single conversion
    while(!(ADCSRA & 0x10));    // wait for conversion done, ADIF flag active
        
    for(i=0;i<8;i++)            // do the ADC conversion 8 times for better accuracy 
    {
        ADCSRA |= (1<<ADSC);        // do single conversion
        while(!(ADCSRA & 0x10));    // wait for conversion done, ADIF flag active
        
        ADC_temp = ADCL;            // read out ADCL register
        ADC_temp += (ADCH << 8);    // read out ADCH register        

        ADCr += ADC_temp;      // accumulate result (8 samples) for later averaging
    }

    ADCr = ADCr >> 3;     // average the 8 samples
        
    cbi(PORTF,PF3); // mt cbi(PORTF, PORTF3);     // disable the VCP
    cbi(DDRF,DDF3); // mt cbi(DDRF, PORTF3);  
    
    cbi(ADCSRA, ADEN);      // disable the ADC

    return ADCr;
}
