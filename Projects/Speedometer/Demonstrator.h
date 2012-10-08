// Demonstrator.h Speedometer version

#include <avr/signal.h>
#include <inttypes.h>

void initializer(void);
void parseInput(char *);

int parse_set(char *);
void set_speed(int);
void sendSpeed(void);

void milliSecInit(unsigned char count);
void setOCR0A(unsigned char count);

