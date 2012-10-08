// Demonstrator.h PWM version

#include <avr/signal.h>
#include <inttypes.h>

void initializer(void);
void parseInput(char *);

int parse_ctc(char *);
void set_ctc(int);

void pwmInit(unsigned char count);
void setOCR0A(unsigned char count);
