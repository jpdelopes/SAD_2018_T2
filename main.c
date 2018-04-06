#include <p24fxxxx.h>
#include <stdio.h>
#include "io_mapping.h"
#include "./adc.h"
#include "./uart.h"

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
	float res;
	unsigned char measureP[6], measureT[6], measureL[6], password[1]="A", ch;

	initADC();
	initUART();
	
	putStringUART("1: Press button RD5:");
	while(PORTDbits.RD5);

	while(1)
	{
		putStringUART("2: Input the 5 character password:");
		ch=getCharUART();

		if(strcmp(ch,password)==0)
			break;
		else
			putStringUART("Wrong password, please try again:");
	}

	putStringUART("3: Press P, T or L:");
	while(1)	
	{
		res = (float)readADC(POT);
		res = (res*5.0)/1024;
		sprintf(measureP, "Pot:  %.2f V", res);

		res = (float)readADC(TEMP);
		res = ((res * 5000 + 512) / 1024) - 500;
		sprintf(measureT, "Temp: %.2f ºC", res);
		
		res = res = (float)readADC(LDR);
		res = (res*5.0)/1024;
		sprintf(measureL, "Temp: %.2f V", res);		

		if(IFS1bits.U2RXIF == 1)			//Receive Interrpt flag
		{
			ch = getCharUART();
			switch (ch)
			{
				case 'P' : putStringUART(measureP); break;
				case 'T' : putStringUART(measureT); break;
				case 'L' : putStringUART(measureL); break;
			}
		}
	}
	return 0;
}

