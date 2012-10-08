// Demonstrator.c scroller demo version

#include "PC_Comm.h"
#include "Demonstrator.h"
#include "LCD_driver.h"
#include "LCD_functions.h"
#include "eeprom.h"
#include "eeprom169.h"

// identify yourself specifically	
const char TALKING_TO[] PROGMEM = "\r\rYou are talking to the \0";
const char WHO_DEMO[] PROGMEM = "'scroller' demo.\r\r\0";	

// bad command
const char BAD_COMMAND1[] PROGMEM = "\rYou sent: '\0";
const char BAD_COMMAND2[] PROGMEM = "' - I don't understand.\r\0";

const char Scroller_msg[] PROGMEM = "Scroller demo\r\0";
const char OnMSG_msg[] PROGMEM = "Loading the message to scroll\r\0";
const char INVALID_msg[] PROGMEM = "Invalid message in EEPROM\r\0";
	
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
	
	showEEPROM();


}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			{
				sendFString(TALKING_TO);
				sendFString(WHO_DEMO);
			}
			break;
		case 'M':
			if( (s[1] == 'S') && (s[2] == 'G'))
				OnMSG(s);
			break;		
		default:
			sendFString(BAD_COMMAND1);
			sendChar(s[0]);
			sendFString(BAD_COMMAND2);

			break;
	
	s[0] = '\0';
	}
}


void OnMSG(char *pMSG)
{
 	static uint8_t temp_index = 0;
	uint16_t lenAddr=EEPROM_START;
	
	sendFString(OnMSG_msg);
	
	while(pMSG[temp_index++] != '\0'); // get the string length
	
  	eeprom_write_byte_169(&lenAddr,temp_index); // store string length
    StoreEEPROM(pMSG, temp_index, EEPROM_START + 1);  //store the string in EEPROM
   
	showEEPROM();
}

void showEEPROM()
{
    static char buffer[64]; // hard coding: Bad coder, bad, bad, bad	
	const uint16_t lenAddr=EEPROM_START;
	uint8_t indexps = 0;
	
 	indexps=eeprom_read_byte_169(&lenAddr); // get the string length
	
	if( (indexps > 0) && (indexps <= 64) ) // a valid length?
	{
		LoadEEPROM(buffer, indexps, EEPROM_START + 4);  // Load message the + 4 skips the MSG 
		LCD_puts(buffer, 1);
	}
	else
	{
		LCD_puts_f(INVALID_msg, 1); // if not, show alternate message
	}
}
