// Demonstrator.h Motor Speed Control version

#include <avr/signal.h>
#include <inttypes.h>

void initializer(void);
void parseInput(char *);

int parse_set(char *);
void set_speed(int);
void sendRPM(void);

void milliSecInit(unsigned char count);
void setOCR0A(unsigned char count);

