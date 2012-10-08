// Demonstrator.c LightMeter version
#include <avr/io.h>

#include "PC_Comm.h"

#include "Demonstrator.h"

void initializer()
{

	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
	// Initialize the ADC to the light sensor channel
	ADC_init(2);
	
	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("You are talking to the LightMeter demo.\r");

}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 'l':
			if( (s[1] == 'i') && (s[2] == 'g') && (s[3] == 'h')  && (s[4] == 't'))
			getLight();
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendString("You are talking to the LightMeter demo.\r");
			break;
		default:
			sendString("\rYou sent: '");
			sendChar(s[0]);
			sendString("' - I don't understand.\r");
			break;
		
	}
	s[0] = '\0';
}

void getLight()
{
	char light[]= {'0','0','0','\0'};

	int ADCresult = 0;

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

// Let's just reuse some code from the Butterfly, 
// you know enough by now to figure it all out:
int ADC_read(void)
{
    char i;
    int ADC_temp;
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


