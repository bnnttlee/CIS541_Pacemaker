// Demonstrator.c Real Time Clock version

#include "PC_Comm.h"
#include "Messages.h"

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
		
	// Display instructions on PC
	sendFString(TALKING_TO);
	sendFString(WHO_DEMO);
	sendFString(ENTER);
	sendFString(TEXT_GET);
	sendFString(ENTER);	
	sendFString(TEXT_SEC);	
	sendFString(TEXT_TOXX);
	sendFString(ENTER);	
	sendFString(TEXT_MIN);	
	sendFString(TEXT_TOXX);
	sendFString(ENTER);	
	sendFString(TEXT_HOUR);	
	sendFString(TEXT_TOXX);
	
}

void parseInput(char s[])
{

	// parse first character	
	switch (s[0])
	{
		
		case 'g':
			if( (s[1] == 'e') && (s[2] == 't') )
			
			showClock();
			break;				
		case 's':
			if( (s[1] == 'e') && (s[2] == 'c') )
			setSecond(s);
			break;				
		case 'm':
			if( (s[1] == 'i') && (s[2] == 'n') )
			setMinute(s);
			break;				
		case 'h':
			if( (s[1] == 'o') && (s[2] == 'u') && (s[3] == 'r'))
			setHour(s);
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

void setSecond(char s[])
{
	char str[] = {0,0,'\0'};
	int sec;
	
	str[0] = s[3];
	str[1] = s[4];
	
	sec = atoi(str);
	if( sec <= 60)
	{
		gSECOND = (uint8_t)sec;
	}
	else
	{
		sendFString(ERROR_NUMBER);
		sendFString(ERROR_60);
	}
}

void setMinute(char s[])
{
	char str[] = {0,0,'\0'};
	int min;
	
	str[0] = s[3];
	str[1] = s[4];
	
	min = atoi(str);
	if( min <= 60)
	{
		gMINUTE = (uint8_t)min;
	}
	else
	{
		sendFString(ERROR_NUMBER);
		sendFString(ERROR_60);
	}
}

void setHour(char s[])
{
	char str[] = {0,0,'\0'};
	int hour;
	
	str[0] = s[4];
	str[1] = s[5];
	
	hour = atoi(str);
	if( hour <= 12)
	{
		gHOUR = (uint8_t)hour;
	}
	else
	{
		sendFString(ERROR_NUMBER);
		sendFString(ERROR_12);
	}
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

	sendFString(THE_TIME_IS);
	sendChar(HH);
	sendChar(HL);
	sendChar(':');
	sendChar(MH);
	sendChar(ML);
	sendChar(':');
	sendChar(SH);
	sendChar(SL);
	sendChar('\r');
	
 
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
     // wait for external crystal to stabilise
	for(int i = 0; i < 10; i++)
			_delay_loop_2(30000);   
			
    cli();					// disabel global interrupt

    cbi(TIMSK2, TOIE2);		// disable OCIE2A and TOIE2

    ASSR = (1<<AS2);		// select asynchronous operation of Timer2

    TCNT2 = 0;				// clear TCNT2A
	
	// select precaler: 32.768 kHz / 128 = 1 sec between each overflow
    TCCR2A |= (1<<CS22) | (1<<CS20);
	
	// wait for TCN2UB and TCR2UB to be cleared
    while((ASSR & 0x01) | (ASSR & 0x04));	

    TIFR2 = 0xFF;			// clear interrupt-flags
    sbi(TIMSK2, TOIE2);		// enable Timer2 overflow interrupt

    sei();					// enable global interrupt

    // initial time and date setting
    gSECOND  = 0;
    gMINUTE  = 0;
    gHOUR    = 0;
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

