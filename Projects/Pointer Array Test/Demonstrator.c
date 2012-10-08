// Demonstrator.c Pointer Array Test version

#include "PC_Comm.h"

void initializer()
{
	// Calibrate the oscillator:
    OSCCAL_calibration();   
	// Initialize the USART
	USARTinit();
	
	// say hello
	sendString("\rPointer Array Test.\r\r");   	 

	Test();

}

void parseInput(char s[])
{
	// Do nothing in this test
}
void Test()
{

	// The hard way
	sendChar('H');					
    sendChar('e');
    sendChar('l');
    sendChar('l');
    sendChar('o');
    sendChar(' ');
	sendChar('w');
    sendChar('o');
    sendChar('r');        
 	sendChar('l');
    sendChar('d');
    sendChar('!');        
    sendChar('\r');

	SayHowdy();
	
	Confuseus();
	
	Confuseusmore();

}

void SayHowdy()
{ 
	char greet[] = "Hello, world!\r*";
	
	sendString("\rIn SayHowdy()\r");	

	for(int i =0 ; greet[i] != '*'; i++)
	{
		sendChar(greet[i]);
	}
}

void Confuseus()
{
	char howdy[] = {'h','o','w','d','y','\0'};
	char *confuseus; 		// create a char pointer
	char c; 				// create a char variable;
	char c1, c2;			// and a couple more
	
	sendString("\rIn Confuseus()\r");	
	
	confuseus = &howdy[0]; 	// set confuseus to point to the howdy array;
	confuseus += 4;  		// set it to point to howdy[4]
	c = *confuseus;		// set the contents of c to the contents of howdy[4]
	sendString("c = *confuseus; = ");
	sendChar(c);
	sendChar('\r');
	
	confuseus -= 4; // reset the pointer
	c1 = *(confuseus + 1);
	sendString("c1 = *(confuseus + 1); = ");
	sendChar(c1);
	sendChar('\r');
	
	c2 = *confuseus + 1;
	sendString("c2 = *confuseus + 1; = ");
	sendChar(c2);
	sendChar('\r');

}

void Confuseusmore()
{
	char howdy[] = {'h','o','w','d','y','\0'};
	int i  = 4;
	char c1,c2;
	char* confuseusmore;

	sendString("\rIn Confuseusmore()\r");

	c1 = howdy[i];			// c1 = 'y' using array notation
	sendString("c1 = howdy[i]; = ");
	sendChar(c1);
	sendChar('\r');	
	
	c2 = *(howdy + 4); 		// c2 = 'y' using pointer notation
	sendString("c2 = *(howdy + 4); = ");
	sendChar(c2);
	sendChar('\r');	
	
	confuseusmore = &howdy[i]; 	// confuseusmore points to 'y'
	sendString("confuseusmore = &howdy[i]; = ");
	sendChar(*confuseusmore);
	sendChar('\r');	
	
	confuseusmore = howdy + i - 1; 	// confuseusmore points to 'd'
	sendString("confuseusmore = howdy + i - 1; = ");
	sendChar(*confuseusmore);
	sendChar('\r');	

}




