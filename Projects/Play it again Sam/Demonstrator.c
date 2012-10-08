// Demonstrator.c Play it again Sam version

#include "PC_Comm.h"
#include "Messages.h"

#include "tunes.h"

void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
	// Initialize timer0 to play a tune
	Timer0_Init();
	
	// initialize piezo-element
    sbi(DDRB, 5);               // set OC1A as output
    sbi(PORTB, 5);              // set OC1A high	
	
	// Display instructions on PC
	sendFString(TALKING_TO);
	sendFString(WHO_DEMO);
	sendFString(ENTER);
	sendFString(TEXT_FUR_ELISE);
	sendFString(ENTER);	
	sendFString(TEXT_TURKEY_MARCH);	
	sendFString(ENTER);	
	sendFString(TEXT_MINUET);	
	sendFString(ENTER);	
	sendFString(TEXT_AULD_LANG_SYNE);	
	sendFString(ENTER);	
	sendFString(TEXT_SIRENE1);	
	sendFString(ENTER);	
	sendFString(TEXT_SIRENE2);	
	sendFString(ENTER);	
	sendFString(TEXT_WHISTLE);	
	sendFString(ENTER);	
	sendFString(VOLUME_UP);	
	sendFString(THE_VOLUME);
	sendFString(ENTER);	
	sendFString(VOLUME_DOWN);	
	sendFString(THE_VOLUME);
	sendFString(ENTER);	
	sendFString(STOP);		


}

void parseInput(char s[])
{
	if( (s[0] <= '7') && ( s[0] >= '1') ) // 7 tunes
	{
		startTune(s[0]);
	}
	else
	{
		// parse first character	
		switch (s[0])
		{
			case '+':
				volumeUp();
				break;
			case '-':
				volumeDown();
				break;
			case 's':
				if( (s[1] == 't') && (s[2] == 'o') && (s[3] == 'p'))
				stopTune();
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
}

void volumeUp()
{
    if(Volume >= 250)
        Volume = 250;
    else
        Volume += 50;
      
	OCRA1H = 0;	
    OCRA1L = Volume;

}
void volumeDown()
{
    if(Volume < 11)
        Volume = 6;
    else
        Volume -= 50;   
    
	OCRA1H = 0;
    OCRA1L = Volume;
}

void stopTune()
{
    cbi(TCCR1B, 0);    // stop Playing
    TCCR1A = 0;
    TCCR1B = 0;
    sbi(PORTB, 5);     // set OC1A high
}


void startTune(char tune)
{

	int song = atoi(&tune) - 1;
 
	stopTune();
	Tone = 0;
	Tempo = 0;
	Duration = 0;

	sendFString(TEXT_SONG_TBL[song]);     // Send the song title to the PC
	sendChar('\r');
            
	pSong=(int*)pgm_read_word(&Songs[song]); // looks too complicated..

	Sound_Init();
}


void Sound_Init(void)
{
    TCCR1A = (1<<COM1A1); 		// Set OC1A when upcounting, clear when downcounting
    TCCR1B = (1<<WGM13);       // Phase/Freq-correct PWM, top value = ICR1
    
    sbi(TCCR1B, CS10);         // start Timer1, prescaler(1)    
    
    OCRA1H = 0;					// Set a initial value in the OCR1A-register
    OCRA1L = Volume;			// This will adjust the volume on the buzzer, lower value => higher volume
}

void Play_Tune(void)
{
    int temp_hi;
    
    char loop;
    
    if(!Tone)
    {
        Duration = 0;   
		
		Tempo = (uint8_t)pgm_read_word(pSong + 0); 
		Tempo <<= 1; // compensate for using 2 MHz clock 
        Tone = 1;   //Start the song from the beginning
    }
    
    if(!Tempo)
    {
        if(Duration)        // Check if the length of the tone has "expired"
        {   
            Duration--;
        }
        else if(pgm_read_word(pSong + Tone))  // If not the end of the song
        {
            Duration = ( DURATION_SEED / pgm_read_word(pSong + Tone) );  // store the duration
			Duration <<= 1;// compensate for using 2 MHz clock
        
			Tone++;                     // point to the next tone in the Song-table        
                        
            if( (pgm_read_word(pSong + Tone) == p) | (pgm_read_word(pSong + Tone) == P) ) // if pause
                cbi(TCCR1B, CS10);             // stop Timer1, prescaler(1)    
            else 
                sbi(TCCR1B, CS10);             // start Timer1, prescaler(1)  
                
            cli(); 
            
			temp_hi = pgm_read_word(pSong + Tone);      // read out the PWM-value

            temp_hi >>= 7;// compensate for 2 MHz clock 8; // move integer 8 bits to the right
                
            TCNT1H = 0;                     // reset TCNT1H/L
            TCNT1L = 0;
            
            ICR1H = temp_hi;                // load ICR1H/L       
			ICR1L = pgm_read_word(pSong + Tone);
			ICR1L <<= 1; // compensate for using 2 MHz clock
            
            sei(); 
            
            Tone++;                     // point to the next tone in the Song-table
        }
        else    // the end of song
        {
            Tone++;         // point to the next tone in the Song-table        
            
			loop = (uint8_t)pgm_read_word(pSong + Tone); // get the byte that tells if the song should loop or not
            
            if( loop )  
            {
                Tone = 1;
            }
            else        // if not looping the song
            {
                Tone = 0;
                //gPlaying = FALSE;
                cbi(TCCR1B, 0);                     // stop Playing
                TCCR1A = 0;
                TCCR1B = 0;
                sbi(PORTB, 5);              // set OC1A high
            }
        }
        
		Tempo = (uint8_t)pgm_read_word(pSong + 0);
    }
    else
        Tempo--;
 
}  


void Timer0_Init(void)
{
 	// Initialize Timer0.
	// Used to give the correct time-delays in the song

    // Enable timer0 compare interrupt
	TIMSK0 = (1<<OCIE0A);

    // Sets the compare value
	OCR0A = 38;

	// Set Clear on Timer Compare (CTC) mode, CLK/256 prescaler
	TCCR0A = (1<<WGM01)|(0<<WGM00)|(4<<CS00);
}


SIGNAL(SIG_OUTPUT_COMPARE0)
{

	Play_Tune();

}
