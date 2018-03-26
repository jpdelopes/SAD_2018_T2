#include <p24fxxxx.h>
#include <stdio.h>
#include "io_mapping.h"
#include "adc.h"

// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif

#define _XTAL_FREQ 	8000000
#define POT			5
#define TEMP		4

char busyADC()
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

void openADC(unsigned int configCON1, unsigned int config2, unsigned int
             	config3, unsigned int configport, unsigned int configscan)
{
    AD1PCFG = configport;
    AD1CSSL = configscan;
    AD1CON3 = config3; 
    AD1CON2 = config2;
    AD1CON1 = config1;
    AD1CON1bits.SAMP = config1 >> 1;
}

void initADC(){
	AD1PCFGbits.PCFG5 = 0;			//Analog input AN5
	
	AD1CON2 = 0x0000;				//Vdd, Vss as Vref+, Vref-
	AD1CON3 = 0x0001;				//Manual Sample, Tad = 2 Tcy
	AD1CON1 = 0x0000;				//SAMP bit = 0 ends sampling, starts converting
	
	AD1CSSL = 0;					//No scanned inputs
	AD1CON1bits.ADON = 1; 			//ADC ON
}

int main(void)
{
	//OSCCON = 0x76; // 8MHz Oscillator
	unsigned int res=0;
	float voltage;
	unsigned char pot[20];

	initADC();
	while(1)	{
		convertADC();
		res = readADC();
		voltage = (res*5.0)/1024;
		printf("%d", res);
		sprintf(pot, "%.3g", res);
	}
	return 0;
}

