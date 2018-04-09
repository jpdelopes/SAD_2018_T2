#include <p24fxxxx.h>
#include <stdio.h>
#include <ctype.h>

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

#define _password 			"123456"
#define _start				"1: Press button RD13: \n\r"
#define _insertPassword 	"2: Input the 5 character password:\n\r"
#define _wrongPassword 		"Wrong password, please try again:\n\r"
#define _correctPassword	"Welcome!\n\r"
#define _options			"3: Press RD6, P, T or L:\n\r"
#define _numCharExceed		"You have exceeded the maximum number of characters!\n\r"

void fanON(float pot_speed)	{
	// if temp > 30ºC -> turn fan on
	// (...)
	Nop();
}

int main(void)
{
	float resP, resT, resLL, resLR;
	char measure[10], ch;
	unsigned int  mode = 1, flag = 1;		//1:Sun and 0:Shadow
	int i = 0;

	LED4_TRIS = 0;	//D7 
	LED5_TRIS = 0;	//D8

	initADC();
	initUART();	

	// ----------------------------------------------------------------------------------
	putStringUART(_start);
	while(PORTDbits.RD13);
	while(U2STAbits.URXDA)					//To empty the buffer
	   getCharUART();

	// ----------------------------------------------------------------------------------
	putStringUART(_insertPassword);
	do
	{
		ch = getCharUART();
		putCharUART(ch);
		switch(ch)
		{
			case 0x0D : flag = strcmp(measure, _password);		// Enter
						if (flag != 0)
						{
							putStringUART(measure);
							putStringUART(_wrongPassword);
							for(i = 9; i > 0; i--)
							   measure[i] = '\0'; 
							measure[0] = '\0';
						}
						break;
			case 0x08 : if(i!=0)								// Backspace
						{
							i--;									
							measure[i] = '\0';
						}
						break;	
			default   : if (i < 10)	
						{
			   				measure[i] = ch;
			   				i++;
						}
						else
						   putStringUART(_numCharExceed);
						break;
		}
	}
	while(flag != 0);
	putStringUART(_correctPassword);

	// ----------------------------------------------------------------------------------
	putStringUART(_options);
	while(1)	
	{
		resP  = (float)readADC(POT);
		resP  = (resP*5.0)/1024;

		resT = (float)readADC(TEMP);
		resT = (resT-155)*0.322;

		// ------------------------------------------------------------------------------
		if (resT > 30)
			fanON(resP);
		else
			Nop();					//Turn off fan
		
		// ------------------------------------------------------------------------------
		if(!BUTTON6_IO)			//The mode changes
		{
			while(!BUTTON6_IO)
			      ;
			mode = (mode == 1) ? 0 : 1;
			if (mode == 0)	
			   putStringUART("Tanning Mode\n\r");
			else
			   putStringUART("Shade Mode\n\r");		
		}

		resLL = (float)readADC(LDR_L);
		resLR = (float)readADC(LDR_R);

		if (resLL == resLR)				//Approximately
		{
			Nop(); //Stop Motor
		}
		else	
		{
			if(mode == 1)			//Try to find the sun
			{
				if (resLL > resLR)		//Move motor to left
					{
					LED4_IO= 1;	//D7 
					LED5_IO= 0;	//D8
					for( i = 0 ; i < 20000 ; i++)
						;
					LED4_IO= 0;	//D7 
					LED5_IO= 0;	//D8
					}

				else{
					LED4_IO= 1;	//D7 
					LED5_IO= 0;	//D8
					for( i = 0 ; i < 20000 ; i++)
						;
					LED4_IO= 0;	//D7 
					LED5_IO= 0;	//D8
				}

			}
			else					//Move away from the sun
				if (resLL > resLR)		//Move motor to right
					{
					LED4_IO= 0;	//D7 
					LED5_IO= 1;	//D8
					for( i = 0 ; i < 20000 ; i++)
						;
					LED4_IO= 0;	//D7 
					LED5_IO= 0;	//D8
					}

				else{
					LED4_IO= 0;	//D7 
					LED5_IO= 1;	//D8
					for( i = 0 ; i < 20000 ; i++)
						;
					LED4_IO= 0;	//D7 
					LED5_IO= 0;	//D8
				}
		}

		// ------------------------------------------------------------------------------
		if(IFS1bits.U2RXIF == 1)	//Receive Interrpt flag
		{
			ch = getCharUART();
			switch (toupper(ch))
			{
				case 'P' : sprintf(measure,"Pot:      %.2f     V\n\r", resP);
						   putStringUART(measure);   
						   break;
				case 'T' : sprintf(measure,"Temp:     %.2f\xf8   C\n\r", resT);
						   putStringUART(measure);  
						   break;
				case 'L' : sprintf(measure,"LDR-L:    %.2f     V\n\r", (resLL*5.0)/1024);
						   putStringUART(measure);
						   sprintf(measure,"LDR-R:    %.2f     V\n\r", (resLR*5.0)/1024); 
						   putStringUART(measure); 
						   break;
			}
		}

	}
	return 0;
}
