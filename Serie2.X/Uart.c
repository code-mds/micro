/*
 * Title: Uart.c
 * Description: Uart configuration function and Uart control
 * Author: CTa
 * Created on 13. ottobre 2019, 09:00*/

#include <p32xxxx.h>
#include "Uart.h"
//#include <plib.h>

unsigned int PbusClock = 20000000;
unsigned int UartBrg = 0;

extern char flagRX;
extern char strg[80]; // global because used by ISR
extern int j;              // global because used by ISR

void UART_ConfigurePins()   //used to configure UART4 TX and RX
{
	TRISFbits.TRISF12 = 0;   //TX digital output
    RPF12R = 2;     // 0010 U4TX - Mapping U4TX to RPF12;
    
    TRISFbits.TRISF13  = 1;   //RX digital input
    U4RXR = 9;     // 1001 RF13 - Mapping U4RX to RPF13
}

void UART_ConfigureUart(int baud)
{
	U4MODEbits.ON     = 0;
    U4MODEbits.SIDL   = 0;
    U4MODEbits.IREN   = 0; 
    U4MODEbits.RTSMD  = 0;
    U4MODEbits.UEN0   = 0; 
    U4MODEbits.UEN1   = 0;
    U4MODEbits.WAKE   = 0;
    U4MODEbits.LPBACK = 0; 
    U4MODEbits.ABAUD  = 0;
    U4MODEbits.RXINV  = 0; 
    U4MODEbits.PDSEL1 = 0; 
    U4MODEbits.PDSEL0 = 0; 
    U4MODEbits.STSEL  = 0;  
    
    U4MODEbits.BRGH   = 0; 

    /* calculate brg */
    UartBrg = (int)(((float)PbusClock/(16 * baud) - 1) + 0.5 );
    U4BRG = UartBrg;

    U4STAbits.UTXEN    = 1;
    U4STAbits.URXEN    = 1;
    U4MODEbits.ON      = 1;     
    
}


int putU4( int c)
{
while(U4STAbits.UTXBF == 1);
    U4TXREG = c;
} // putU2

char getU4( void)
{
//RTS=0; // assert Request To Send !RTS

while (!U4STAbits.URXDA); // wait for a new char to arrive
//RTS=1;
return U4RXREG; // read char from receive buffer
} // getU4

void putU4_string(char szData[])
{
    char *pData = szData;
    while(*pData)
    {
        putU4((*(pData++)));
    }
}

unsigned char getU4_string()
{
     unsigned char c;
     while(U4STAbits.URXDA)
        {
            //c = (unsigned char)U4RXREG;
            c = getU4();
            strg[j++] = c;      
            if(j>0 && c == '\r')
            {
                strg[j-1] = 0;
                j = 0;
                flagRX = 1;    
            }       
        }         
}

