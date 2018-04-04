#include "uart.h"

void initUART(void)	
{
	// Set directions of UART IOs
	//UART_TX_TRIS = 0;
	//UART_RX_TRIS = 1;

	U2BRG = BAUDRATE;
	U2MODE = 0x8000;			//8-bit, no parity, 1 stop bit
	U2STA = 0;

	IFS1bits.U2TXIF = 0;		//Interrupt flag
	IFS1bits.U2RXIF = 0;
	
	//U2MODEbits.UARTEN = 1;	//Enable module
	U2STAbits.UTXEN = 1;		//Enable transmission
}

void  putCharUART(char ch)
{
    while(CTS);					//Clear to send
    while(U2STAbits.UTXBF);		//TX buffer full
      U2TXREG = ch;
}

void putStringUART(char* str)
{
	while(*str)					//Until EOS: *s == '\0'
	{
		putCharUART(*str++);
	}
	putCharUART('\r');	
}

char getCharUART(void)
{
	while(!U2STAbits.URXDA);	//URXDA bit: Buffer has data.
	IFS1bits.U2RXIF = 0;
	return U2RXREG;
}