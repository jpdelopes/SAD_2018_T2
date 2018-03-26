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

#define BAUDRATE 		25
#define UART_TX_TRIS 	TRISFbits.TRISF5
#define UART_RX_TRIS 	TRISFbits.TRISF4

void initUART(void)	
{
	// Set directions of UART IOs
	UART_TX_TRIS = 0;
	UART_RX_TRIS = 1;
	U2BRG = BAUDRATE;
	U2MODE = 0;				//8-bit, no parity, 1 stop bit
	U2STA = 0;
	U2MODEbits.UARTEN = 1;	//Enable module
	U2STAbits.UTXEN = 1;	//Enable transmission
  	// reset RX flag
 	IFS1bits.U2RXIF = 0;
}

void  UART2PutChar(char Ch){
    // wait for empty buffer  
    while(U2STAbits.UTXBF == 1);
      U2TXREG = Ch;
}

char UART2IsPressed()
{
    if(IFS1bits.U2RXIF == 1)
        return 1;
    return 0;
}

char UART2GetChar(){
	char Temp;
    while(IFS1bits.U2RXIF == 0);
    Temp = U2RXREG;
    IFS1bits.U2RXIF = 0;
    return Temp;
}

void  UART2PutDec(unsigned char Dec){
unsigned char Res;
    Res = Dec;

    if(Res/100) 
        UART2PutChar(Res/100+'0');
    Res = Res - (Res/100)*100;

    if(Res/10) 
        UART2PutChar(Res/10+'0');
    Res = Res - (Res/10)*10;
 
    UART2PutChar(Res+'0');
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
		//printf("%d", res);
		//sprintf(pot, "%.3g", res);
		initUART();
		if(UART2IsPressed()){
			UART2PutDec(voltage);
		}
	}
	return 0;
}

