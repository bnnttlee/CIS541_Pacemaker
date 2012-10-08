// Demonstrator.h ADC version

#include <avr/signal.h>
#include <inttypes.h>

#include <avr/pgmspace.h>

void initializer(void);
void parseInput(char *);
void getLight(void);
void getTemperature(void);
void getVolt(void);
void ADC_init(char);
int ADC_read(void);
