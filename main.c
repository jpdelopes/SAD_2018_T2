#include <p24fxxxx.h>
#include <stdio.h>
#include <string.h>
#include "system_config/io_mapping.h"

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

LED0_TRIS = 0;
LED1_TRIS = 0;
LED2_TRIS = 0;
LED3_TRIS = 0;
LED4_TRIS = 0;
LED5_TRIS = 0;
LED6_TRIS = 0;



	int i = 0;

	while ( 1 ){
		if ( !PORTDbits.RD7 ){
			LED0_IO= 1;
			for( i = 0 ; i < 1000 ; i++){};
			LED1_IO= 1;
			for( i = 0 ; i < 1000 ; i++){};
			LED0_IO= 0;
			for( i = 0 ; i < 20000 ; i++){};
			LED2_IO= 1;
			for( i = 0 ; i < 1000 ; i++){};
			LED1_IO= 0;
			for( i = 0 ; i < 1000 ; i++){};
			LED3_IO= 1;
			for( i = 0 ; i < 20000 ; i++){};
			LED2_IO= 0;
			for( i = 0 ; i < 1000 ; i++){};
			LED4_IO= 1;
			for( i = 0 ; i < 1000 ; i++){};
			LED3_IO= 0;
			for( i = 0 ; i < 20000 ; i++){};
			LED5_IO= 1;
			for( i = 0 ; i < 1000 ; i++){};
			LED4_IO= 0;
			for( i = 0 ; i < 20000 ; i++){};
			LED5_IO= 0;
			for( i = 0 ; i < 1000 ; i++){};
			LED6_IO= 1;
			for( i = 0 ; i < 20000 ; i++){};
			LED5_IO= 1;
			for( i = 0 ; i < 20000 ; i++){};
			LED6_IO= 0;
			LED4_IO= 1;
LED5_IO= 0;
			for( i = 0 ; i < 1000 ; i++){};
LED3_IO= 1;
			for( i = 0 ; i < 20000; i++){};
LED4_IO= 0;
			for( i = 0 ; i < 1000 ; i++){};
LED2_IO= 1;
			for( i = 0 ; i < 20000 ; i++){};
LED3_IO= 0;
			for( i = 0 ; i < 1000 ; i++){};
LED1_IO= 1;
			for( i = 0 ; i < 20000; i++){};
LED0_IO= 1;
			for( i = 0 ; i < 1000 ; i++){};
LED0_IO= 0;
			for( i = 0 ; i < 20000; i++){};
LED2_IO= 0;
			for( i = 0 ; i < 20000; i++){};
LED1_IO= 0;
			for( i = 0 ; i < 20000; i++){};
LED0_IO= 0;
			for( i = 0 ; i < 1000 ; i++){};




		}
	}
}
