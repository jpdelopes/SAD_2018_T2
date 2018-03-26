#include <p24fxxxx.h>
#include <stdio.h>
#include "io_mapping.h"
#include "./adc.h"

// Configuration Bits
#ifdef __PIC24FJ64GA004__ //Defined by MPLAB when using 24FJ64GA004 device
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx1 & IOL1WAY_ON) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI & I2C1SEL_SEC)
#else
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRI)
#endif

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
		//printf("%d", res);
		//sprintf(pot, "%.3g", res);
	}
	return 0;
}

