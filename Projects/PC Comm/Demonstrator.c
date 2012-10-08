// Demonstrator.c CommDemo version

#include "PC_Comm.h"

void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   
	// Initialize the USART
	USARTinit();
	
	// say hello
	sendString("\rPC_Comm.c ready to communicate.\r");   	 
	// identify yourself specifically
	sendString("\rYou are talking to the PC_Comm demo.\r");

}

void parseInput(char s[])
{
	// parse first character	
	switch (s[0])
	{
		case 'c':
			if( (s[1] == 'o') && (s[2] == 'm') && (s[3] == 'm') )
			switch (s[4]) // parse the fifth character
			{
				case 'a':
					Comm1(s);
					break;
				case 'b':
					Comm2(s);
					break;
				case 'c':
					Comm3(s);
					break;
				case 'd':
					Comm4(s);
					break;
				default:
					sendString("\rYou sent: '");
					sendChar(s[0]);
					sendString("' - I don't understand.\r");	
			}
			break;
		case 'd':
			if( (s[1] == 'e') && (s[2] == 'm') && (s[3] == 'o') && (s[4] == '?') )
			sendString("You are talking to the PC_Comm demo.\r");
			break;
		case 'h':
			if( (s[1] == 'e') && (s[2] == 'l') && (s[3] == 'l') && (s[4] == 'o') )
			sendString("Hello yourself\r");
			break;
		default:
			sendString("\rYou sent: '");
			sendChar(s[0]);
			sendString("' - I don't understand.\r");
			break;
		
	}
	s[0] = '\0';
}

void Comm1(char s[])
{
	responder(s,s[4]);
}

void Comm2(char s[])
{
	responder(s,s[4]);
}

void Comm3(char s[])
{
	responder(s,s[4]);
}

void Comm4(char s[])
{
	responder(s,s[4]);
}

void responder(char s[], char c)
{
	char sComm[11];
	unsigned char i = 5, j = 0;

	while( (s[i] != '\0') && (j <= 11) ) 
	{
	
		if( (s[i] >= '0') && (s[i] <= '9') )
		{
			sComm[j++] = s[i++];
		}
		else
		{
			sendString("Error - Comm");
			sendChar(c);
			sendString(" received a non integer: ");
			sendChar(s[i]);
			sendChar('\r');
		}
	}
	
	sComm[j] = '\0';
	
	if(j>11)
	{
		sendString("Error - Comm");
		sendChar(c);
		sendString(" number too large\r");
		sendChar('\r');
	}
	else
	{
		sendString("\rThank you for sending the number: ");
		sendString(sComm);
		sendChar('\r');
	}
}



