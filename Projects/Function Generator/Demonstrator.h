// Demonstrator.h Function Generator / Digital Oscilloscope version

void initializer(void);
void parseInput(char *);
void showMessage(char);


int parse_ctc(char *);
void set_ctc(int);

void startWave(int);
void startSine(void);
void startSquare(void);
void startSawtooth(void);
void startTriangle(void);

void DigitalOscilloscopeTimerInit(void);

void MilliSec_init(unsigned char count);
void set_OCR0A(unsigned char count);

void ADC_init(void);
int ADC_read(void);
