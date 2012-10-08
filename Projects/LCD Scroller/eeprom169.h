/* 
	Workaround since the avr-libc has no support
	for the ATmega169 eeprom at the time of 
	writing this (avr-libc V1.0) 

	This is based on the ATMEL Databook for the m169, 
	with some changes in the interfaces and interrupt
	control
	
	done by Martin Thomas, KL, .de
*/

#ifndef _eeprom168_h_
#define _eeprom168_h_

#include <inttypes.h>
#include <avr/interrupt.h>

static inline uint8_t eeprom_read_byte_169(const uint16_t *addr)
{
	// cli();
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE)) asm volatile ("nop"::);
	/* Set up address register */
	EEAR = *addr;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	// sei();
	return EEDR;
}

static inline void eeprom_write_byte_169(uint16_t *addr, uint8_t val) 
{
	cli();
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE)) asm volatile ("nop"::);
	/* Set up address and Data Registers */
	EEAR = *addr;
	EEDR = val;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
	while(EECR & (1<<EEWE)) asm volatile ("nop"::);
	sei();
}

#endif
