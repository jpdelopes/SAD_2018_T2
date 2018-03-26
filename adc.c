#include "adc.h"

char busyADC(void)
{
	return !(AD1CON1bits.DONE);	
}

void setChanADC(unsigned int ch)
{
    AD1CHS = ch;
}

void convertADC(void)
{
	AD1CON1bits.SAMP = 0;
}

unsigned int readADC(void)
{
	int i;

	//Switch Channel AD1CHS = channel;
	AD1CHS  = POT;

	AD1CON1bits.SAMP = 1; 	//Start sampling
	for(i=0;i<1000;i++)
    {
        Nop(); 			 	//Sample delay
    } 
	convertADC();          
    for(i=0;i<1000;i++)
    {
        Nop(); 			
    }
	while(busyADC());

    return ADC1BUF0;
}

void openADC(unsigned int config1, unsigned int config2, unsigned int
             	config3, unsigned int configport, unsigned int configscan)
{
    AD1PCFG = configport;
    AD1CSSL = configscan;
    AD1CON3 = config3; 
    AD1CON2 = config2;
    AD1CON1 = config1;
    AD1CON1bits.SAMP = config1 >> 1;
}

void initADC()
{
	AD1PCFGbits.PCFG5 = 0;			//Analog input AN5
	AD1PCFGbits.PCFG4 = 0;

	AD1CON2 = 0x0000;				//Vdd, Vss as Vref+, Vref-
	AD1CON3 = 0x0001;				//Manual Sample, Tad = 2 Tcy
	AD1CON1 = 0x0000;				//SAMP bit = 0 ends sampling, starts converting
	
	AD1CSSL = 0;					//No scanned inputs
	AD1CON1bits.ADON = 1; 			//ADC ON
}