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

unsigned int readADC(unsigned int ch)
{
	int i;

	setChanADC(ch);
	for(i=0;i<1000;i++)
    {
        Nop(); 			
    }
	AD1CON1bits.SAMP = 1; 	//Start sampling
	for(i=0;i<1000;i++)
    {
        Nop(); 			 	//Sample delay
    } 
	convertADC();           //Start conversion
	while(busyADC());

    return ADC1BUF0;
}

void initADC()
{
	AD1PCFGbits.PCFG5 = 0;			//Analog input AN5
	AD1PCFGbits.PCFG4 = 0;			//AN4
	AD1PCFGbits.PCFG6 = 0;		
	AD1PCFGbits.PCFG7 = 0;			

	AD1CON3 = 0X0002; 				//Manual Sample, Tad = 2 Tcy
	AD1CON2 = 0x0000;				//Vdd, Vss as Vref+, Vref-
	AD1CON1 = 0;					//SAMP bit = 0 ends sampling, starts converting
	
	AD1CSSL = 0;					//No scanned inputs
	AD1CON1bits.ADON = 1; 			//ADC ON
}
