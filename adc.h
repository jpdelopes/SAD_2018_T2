#include <p24fxxxx.h>

#define _XTAL_FREQ 	8000000
#define POT			5
#define TEMP		4
#define LDR_L		6
#define LDR_R		7
#define MOTOR		3

char busyADC(void);

void setChanADC(unsigned int ch);

void convertADC(void);

unsigned int readADC(unsigned int ch);

void openADC(unsigned int config1, unsigned int config2, unsigned int
             	config3, unsigned int configport, unsigned int configscan);

void initADC();

