// Demonstrator.c ADC version

#include "PC_Comm.h"
#include "Demonstrator.h"

//WHERE DOES THIS GO? 
	/*First, we set the ATmega169 ADC multiplexer to connect
	the ADC to pin 61 (ADC0) using the ADMUX register (databook p 207).
	The Butterfly has the ATmega169 pin 62, (AREF) connected to a bypass
	capacitor to help lessen noise on the ADC, so we set the ADMUX 
	bits 6 and 7 to 0 to select the 'use external reference' option and 
	set all the rest to select the ACD multiplexer input channel.*/
	
	
void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   
	
	// Initialize the USART
	USARTinit();
	
	ADC_init(1);
	
	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("You are talking to the ADC demo.\r");
	// show commands
	sendString("Commands:\r");
	sendString("light - returns a light value\r");
	sendString("temp - returns the temperature in fahrenheit\r");
	sendString("volt - returns a voltage value\r");
	
}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 'l':
			if( (s[1] == 'i') && (s[2] == 'g')&& (s[3] == 'h') && (s[4] == 't'))
			getLight();
			break;
		case 't':
			if( (s[1] == 'e') && (s[2] == 'm')&& (s[3] == 'p'))
			getTemperature();
			break;
		case 'v':
			if( (s[1] == 'o') && (s[2] == 'l')&& (s[3] == 't'))
			getVolt();
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendString("You are talking to the ADC demo.\r");
			break;
		default:
			sendString("\rYou sent: '");
			sendChar(s[0]);
			sendString("' - I don't understand.\r");
			break;
		
	}
	s[0] = '\0';
}



/**********************************************************************************
	Volt Meter 
***********************************************************************************/
void getVolt()
{
// BIZARRO WORLD ERROR - ORIGINAL ARRAYS TOO SMALL SEEMED TO STEP ON BAUDRATE!!!
// HALF DAYS WORK DEBUGGING ITOA!!!!!!!!!!!
// BORROWED CODE: char voltintpart[]= {'0','\0'}; SCREWED UP
	char voltintpart[]= {'0','0','0','\0'};
	char voltfractpart[]= {'0','0','0','\0'};
	int intpart = 0;
	int fractpart = 0;
	int ADCresult = 0;

	ADCresult = ADC_read();	
	intpart = ADCresult/50;
	fractpart = ADCresult%50;

	itoa(intpart, voltintpart, 10);
	itoa(fractpart, voltfractpart, 10);

	// Send the voltage to the PC
	sendString("The reading is ");
	sendChar(voltintpart [0]);
	sendChar('.');
	sendChar(voltfractpart [0]);
	sendString(" volts.\r");	
}

/**********************************************************************************
	Lightmeter
***********************************************************************************/
void getLight()
{
	char light[]= {'0','0','0','\0'};
	int ADCresult = 0;

	// Initialize the ADC to the light sensor channel
	//ADC_init(2);	
	ADMUX = 2;//input;
	
	ADCresult = ADC_read();

	/* Next we convert the integer ADCresult to a string that we can transmit to the PC. 
	Lets use a function from the standard library. We add #include <stdlib.h> to our file. 
	Then we can use the itoa() function, which converts and integer to an ASCII character 
	array terminated with '\0'. */ 	
	itoa(ADCresult, light, 10);
	
	// Send the temperature to the PC
	sendString("The light reading is ");
	sendString(light);
	sendString(" somethings.\r");

}

/**********************************************************************************
	Digital Thermometer
***********************************************************************************/
// mt __flash int TEMP_Farenheit_pos[] =  // Positive Farenheit temperatures (ADC-value)
const int TEMP_Fahrenheit_pos[] PROGMEM =  // Positive Fahrenheit temperatures (ADC-value)
        {                           // from 0 to 140 degrees
		    938, 935, 932, 929, 926, 923, 920, 916, 913, 909, 906, 902, 898, 
    		894, 891, 887, 882, 878, 874, 870, 865, 861, 856, 851, 847, 842, 
    		837, 832, 827, 822, 816, 811, 806, 800, 795, 789, 783, 778, 772, 
    		766, 760, 754, 748, 742, 735, 729, 723, 716, 710, 703, 697, 690, 
    		684, 677, 670, 663, 657, 650, 643, 636, 629, 622, 616, 609, 602, 
    		595, 588, 581, 574, 567, 560, 553, 546, 539, 533, 526, 519, 512, 
    		505, 498, 492, 485, 478, 472, 465, 459, 452, 446, 439, 433, 426, 
    		420, 414, 408, 402, 396, 390, 384, 378, 372, 366, 360, 355, 349, 
    		344, 338, 333, 327, 322, 317, 312, 307, 302, 297, 292, 287, 282, 
    		277, 273, 268, 264, 259, 255, 251, 246, 242, 238, 234, 230, 226, 
    		222, 219, 215, 211, 207, 204, 200, 197, 194, 190, 187, 
        };
		
void getTemperature()
{
	char fahr[]= {'0','0','0','\0'};

	int ADCresult = 0;
	int i = 0;
	
	// Initialize the ADC to the temperature sensor channel
	//ADC_init(0);	
	ADMUX = 0;//input;
	
	ADCresult = ADC_read();

	/* The pgm_read_word() function is part of WinAVR and reads a word from the program
	space with a 16-bit (near) address, as in the table.  When a table entry is found 
	that is less than the ADC result we break and i equals the temperature in Fahrenheit. 
	Pretty clever, huh? Wish I thought of it, but I borrowed it from the WinAVR version 
	of the Butterfly code. I'll quit owning up to all this theft and from now on 
	if you see something clever (the good kind of clever) just assume that I stole it. */
	for (i=0; i<=141; i++)  
    	{
		if (ADCresult > pgm_read_word(&TEMP_Fahrenheit_pos[i]))
        	{
            break;
        }
    }
	
	/* Next we convert the integer ADCresult to a string that we can transmit to the PC. 
	Lets use a function from the standard library. We add #include <stdlib.h> to our file. 
	Then we can use the itoa() function, which converts and integer to an ASCII character 
	array terminated with '\0'. */ 	
	itoa(i, fahr, 10);
	
	// Send the temperature to the PC
	sendString("The temperature is ");
	sendString(fahr);
	sendString(" degrees Fahrenheit.\r");

}

/**********************************************************************************
	ADC common functions
***********************************************************************************/
/*void ADC_init(char input)
{
	// set the mutiplexer to use the channel given by 'input'
    //ADMUX = input;    // external AREF and ADCx

    // set ADC prescaler to , 1MHz / 8 = 125kHz    
    //ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);
    
    // set ADC prescaler to , 2MHz / 16 = 125kHz    
    ADCSRA = (1<<ADEN) | (1<<ADPS2);    

    // Take a dummy reading , which basically allows the ADC 
    // to hack up any hairballs before we take any real readings
    input = ADC_read();       
}
*/
void ADC_init(char input)
{
	/*First, we set the ATmega169 ADC multiplexer to connect
	the ADC to pin 61 (ADC0) using the ADMUX register (databook p 207).
	The Butterfly has the ATmega169 pin 62, (AREF) connected to a bypass
	capacitor to help lessen noise on the ADC, so we set the ADMUX 
	bits 6 and 7 to 0 to select the 'use external reference' option and 
	set all the rest to select the ACD multiplexer input channel.*/
    ADMUX = input;    // external AREF and ADCx

    // set ADC prescaler to , 1MHz / 8 = 125kHz    
    ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);    

    //  Take a dummy reading , which basically allows the ADC 
    // to hack up any hairballs before we take any real readings
    input = ADC_read();       
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



