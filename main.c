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
#define POT			0x0005

char busyADC()
{
	return !(AD1CON1bits.DONE);	
}

void initADC(){
/*
c) Select the analog conversion clock to
match the desired data rate with the
processor clock (AD1CON3<7:0>).
d) Select the appropriate sample/conversion
sequence (AD1CON1<7:5> and
AD1CON3<12:8>).
e) Select how conversion results are
presented in the buffer (AD1CON1<9:8>).
f) Select the interrupt rate (AD1CON2<5:2>).
g) Turn on the A/D module (AD1CON1<15>).
*/
	AD1PCFGbits.PCFG5 = 0;			//Analog input AN5

	AD1CON2 = 0x0000;				//Vdd, Vss as Vref+, Vref-
	AD1CON3 = 0x0001;				//Manual Sample, Tad = 2 Tcy
	AD1CON1 = 0x0000;				//SAMP bit = 0 ends sampling, starts converting
	
    AD1CHS  = POT;
	
	AD1CSSL = 0;					//No scanned inputs
	AD1CON1bits.ADON = 1; // turn ADC ON
}

int main(void)
{
	OSCCON = 0x76; // 8MHz Oscillator
	
	initADC();
	//BusyADC10();

	return 0;
}

