#include <p24fxxxx.h>

#define _XTAL_FREQ 	8000000
#define POT			5
#define TEMP		4

char busyADC(void);

void setChanADC(unsigned int ch);

void convertADC(void);

unsigned int readADC(void);

void openADC(unsigned int config1, unsigned int config2, unsigned int
             	config3, unsigned int configport, unsigned int configscan);

void initADC();

