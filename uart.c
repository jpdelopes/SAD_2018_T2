#include "uart.h"

void initUART(void)	
{
	// Set directions of UART IOs
	//UART_TX_TRIS = 0;
	//UART_RX_TRIS = 1;

	U2BRG = BAUDRATE;
	U2MODE = 0x8000;			//8-bit, no parity, 1 stop bit
	U2STA = 0;
	
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
//unsigned char Res;
  //  Res = Dec;
//
 //   if(Res/100) 
  //      UART2PutChar(Res/100+'0');
 //   Res = Res - (Res/100)*100;

  //  if(Res/10) 
   //     UART2PutChar(Res/10+'0');
    //Res = Res - (Res/10)*10;
 
//    UART2PutChar(Res+'0');
}