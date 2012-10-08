// Demonstrator.h Precision Blinking version

#include <avr/signal.h>
#include <inttypes.h>

void initializer(void);
void parseInput(char *);

int parse_ctc(char *);
void set_ctc(int);

void MilliSec_init(unsigned char count);
void set_OCR0A(unsigned char count);
