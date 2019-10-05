#include <p32xxxx.h>
#include "utils_uart.h"

void utils_uart_ConfigurePins() 
{
    TRISFbits.TRISF12 = 0; // TX digital output
    RPF12R = 0b0010;       // mapping U4TX to RPF12
    TRISFbits.TRISF13 = 0; // RX digital output
    U4RXR = 0b1001;        // mapping RPF13 to U4RX
}

void utils_uart_ConfigureUart(int baud) {
    unsigned int PbusClock = 40000000;
    unsigned int UartBrg = 0;
    
    U4MODE = 0x000;
    U4MODEbits.STSEL = 0; // 1 bit stop    
    U4MODEbits.PDSEL = 0; // nessuna parita
            
    UartBrg = (int)(((float)PbusClock/(16*baud)-1)+0.5);
    U4BRG  = UartBrg;
    
    U4STAbits.UTXEN = 1;    // abilita trasmissione
    U4STAbits.URXEN = 1;    // abilita ricezione

    U4MODEbits.ON = 1;      // UART ON
}

int utils_uart_putU4(int c)
{
    while(U4STAbits.UTXBF==1);
    U4TXREG=c;
}
/****************************************************************/
char utils_uart_getU4(void)
{
    while(!U4STAbits.URXDA);//wait for a new char to arrive
    return U4RXREG; //read char from receive buffer
}
/****************************************************************/
void utils_uart_putU4_string(char szData[])
{
    char* pData = szData;
    while(*pData) {
        utils_uart_putU4((*(pData++)));
    }
}

void utils_uart_getU4_string(char buffer[], int sz)
{
    int i=0;
    char ch;
    for(i=0; i<sz || ch == '\n'; i++) {
        ch = utils_uart_getU4();
        buffer[i] = ch;
    }
}
