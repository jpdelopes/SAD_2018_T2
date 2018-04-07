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

void fanON(float pot_speed)	{
	// if temp > 30ºC -> turn fan on
	// (...)
	Nop();
}

int main(void)
{
	float res, res_;
	unsigned char measureP[6], measureT[6], measureLL[6], measureLR[6], password[1]="A", ch;
	int state = 1;		//1:Sun and 0:Shadow

	initADC();
	initUART();	

	// ------------------------------------- 1 ---------------------------------------------
	putStringUART("1: Press button RD13:");
	while(PORTDbits.RD13);

	// ------------------------------------- 2 ---------------------------------------------
	while(1)
	{
		putStringUART("2: Input the 5 character password:");
		ch=getCharUART();

		if(strcmp(ch,password)==0)
			break;
		else
			putStringUART("Wrong password, please try again:");
	}

	// ------------------------------------- 3 ---------------------------------------------
	putStringUART("3: Press RD6, P, T or L:");
	while(1)	
	{
		res  = (float)readADC(POT);
		res  = (res*5.0)/1024;
		sprintf(measureP, "Pot:   %.2f V", res);

		res_ = (float)readADC(TEMP);
		res_ = ((res*5000+512)/1024)-500;
		sprintf(measureT, "Temp:  %.2f ºC", res_);

		// --------------------------------- 5 ---------------------------------------------
		if (res_ > 30)
			fanON(res);
		else
			Nop();					//Turn off fan
		
		res  = (float)readADC(LDR_L);
		sprintf(measureLL, "LDR-L: %.2f V", (res*5.0)/1024);

		res_ = (float)readADC(LDR_R);
		sprintf(measureLR, "LDR-R: %.2f V", (res_*5.0)/1024);		
		
		// --------------------------------- 4 ---------------------------------------------
		if(!PORTDbits.RD6)			//The mode changes
		{
			if(state == 1)
				state = 0;
			else
				state = 1;
		}

		if (res > res_)				//Approximately
		{
			Nop(); //Stop Motor
		}
		else	
		{
			if(state == 1)			//Try to find the sun
			{
				if (res > res_)		//Move motor to left
					Nop(); //!!!
			}
			else					//Move away from the sun
			{
				if (res < res_)		//Move motor to right
					Nop(); //!!!
			}
		}

		// --------------------------------- 5 ---------------------------------------------
		if(IFS1bits.U2RXIF == 1)	//Receive Interrpt flag
		{
			ch = getCharUART();
			switch (ch)
			{
				case 'P' : putStringUART(measureP);  break;
				case 'T' : putStringUART(measureT);  break;
				case 'L' : putStringUART(measureLL); 
						   putStringUART(measureLR); break;
			}
		}

	}
	return 0;
}

