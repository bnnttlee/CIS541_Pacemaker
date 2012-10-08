// PC_Comm.h
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/signal.h>

#include <stdlib.h>
#include <avr/pgmspace.h>

#include "Demonstrator.h"

void OSCCAL_calibration(void) ;

void USARTinit(void);
char isCharAvailable(void);
char receiveChar(void);
void sendChar(char ) ;
void sendString(char *);

void sendFString(const char *);

