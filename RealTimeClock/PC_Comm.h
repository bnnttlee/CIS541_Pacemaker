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

#ifdef UDR
#define UDR0 UDR
#define UCSR0A UCSRA
#define UBRR0H UBRRH
#define UBRR0L UBRRL
#define U2X0 U2X
#define UCSR0B UCSRB
#define RXEN0 RXEN
#define TXEN0 TXEN
#define RXCIE0 RXCIE
#define UDRIE0 UDRIE
#define UCSR0C UCSRC
#define UMSEL0 UMSEL
#define UPM00 UPM0
#define USBS0 USBS
#define UCSZ00 UCSZ0
#define UCPOL0 UCPOL
#endif

#ifndef cbi
#define sbi(port,  bit) (port)|= (1<< (bit))
#define cbi(port, bit) (port) &= ~(1<< (bit))
#endif

 #ifdef OCRA1H
#define OCRA1H OCR1AH
#define OCRA1L OCR1AL
#endif
