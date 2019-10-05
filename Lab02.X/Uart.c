#include <p32xxxx.h>
#include "Uart.h"

void UART_ConfigurePins() 
{
    TRISFbits.TRISF12 = 0; // TX digital output
    RPF12R = 0b0010;       // mapping U4TX to RPF12
    TRISFbits.TRISF13 = 0; // RX digital output
    U4RXR = 0b1001;        // mapping RPF13 to U4RX
}

void UART_ConfigureUart(int baud) {
    unsigned int PbusClock = 40000000;
    unsigned int UartBrg = 0;
    
    U4MODE = 0x000;
    UartBrg = (int)(((float)PbusClock/(16*baud)-1)+0.5);
    U4BRG  = UartBrg;
    
    U4STAbits.UTXEN = 1;    // abilita trasmissione
    U4STAbits.URXEN = 1;    // abilita ricezione

    U4MODEbits.ON = 1;      // UART ON
}

int putU4(int c)
{
    while(U4STAbits.UTXBF==1);
    U4TXREG=c;
}
/****************************************************************/
char getU4(void)
{
    while(!U4STAbits.URXDA);//wait for a new char to arrive
    return U4RXREG; //read char from receive buffer
}
/****************************************************************/
void putU4string(char szData[])
{
    char*pData=szData;
    while(*pData) {
        putU4((*(pData++)));
    }
}
