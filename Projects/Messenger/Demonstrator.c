// Demonstrator.c Messenger version

#include "PC_Comm.h"
#include "Messages.h"

	
void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   

	// Initialize the USART
	USARTinit();
	
	// Display instructions on PC
	sendFString(TALKING_TO);
	sendFString(WHO_DEMO);
	
	char c = '0';
	for(int i = 0; i < 5; i++)
	{
		sendFString(ENTER);
		sendChar(c + i);
		sendFString(FOR);	
		sendFString(ERROR_TBL[i]);
	}

}

void parseInput(char s[])
{
	if( (s[0] <= '4') && ( s[0] >= '0') ) // 5 error messages
	{
		showMessage(s[0]);
	}
	else
	{
		// parse first character	
		switch (s[0])
		{
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

void showMessage(char mess)
{

	int num = atoi(&mess);

	sendFString(ERROR_TBL[num]);     // Send the song title to the PC

}
