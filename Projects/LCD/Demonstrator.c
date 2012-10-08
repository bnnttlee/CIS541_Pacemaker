// Demonstrator.c LCD demo version

#include "PC_Comm.h"
#include "Demonstrator.h"
#include "LCD_test.h"
#include "LCD_driver.h"
#include "LCD_functions.h"

// identify yourself specifically	
const char TALKING_TO[] PROGMEM = "\r\rYou are talking to the \0";
const char WHO_DEMO[] PROGMEM = "'LCD' demo.\r\r\0";	

// bad command
const char BAD_COMMAND1[] PROGMEM = "\rYou sent: '\0";
const char BAD_COMMAND2[] PROGMEM = "' - I don't understand.\r\0";

const char LCD_START_msg[] PROGMEM = "LCD demo\0";
	
void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
	// initialize the LCD
    LCD_Init();                 
	
	// Display instructions on PC
	sendFString(TALKING_TO);
	sendFString(WHO_DEMO);
	
	LCD_puts_f(LCD_START_msg, 1);

}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendFString(TALKING_TO);
			sendFString(WHO_DEMO);
			break;
		case 'C':
			if( (s[1] == 'L') && (s[2] == 'E') && (s[3] == 'A') && (s[4] == 'R'))
				OnCLEAR();
			else if ( (s[1] == 'O') && (s[2] == 'L') && (s[3] == 'O') && (s[4] == 'N'))
				OnCOLON(s);
			break;		
		case 'P' :
			if( (s[1] == 'U') && (s[2] == 'T') && (s[3] == 'C'))
				OnPUTC(s);
			else if( (s[1] == 'U') && (s[2] == 'T') && (s[3] == 'F'))
				OnPUTF(s);
			else if( (s[1] == 'U') && (s[2] == 'T') && (s[3] == 'S'))
				OnPUTS(s);
			break;		
		case 'S' :
			if( (s[1] == 'C') && (s[2] == 'R') && (s[3] == 'O') && (s[4] == 'L') && (s[5] == 'L'))
				OnSCROLL(s);
			else if( (s[1] == 'E') && (s[2] == 'T') && (s[3] == 'C') )
				OnSETC(s);
			break;				
		default:
			sendFString(BAD_COMMAND1);
			sendChar(s[0]);
			sendFString(BAD_COMMAND2);

			break;
		
	
	s[0] = '\0';
	}
}
