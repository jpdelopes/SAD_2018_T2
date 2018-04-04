#include <p24fxxxx.h>

#define BAUDRATE 		12 //25
#define UART_TX_TRIS 	TRISFbits.TRISF5
#define UART_RX_TRIS 	TRISFbits.TRISF4

#define CTS				_RF12

void initUART(void);

void putCharUART(char ch);

void putStringUART(char *str);

char getCharUART(void);