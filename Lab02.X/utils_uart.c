#include <p32xxxx.h>
#include <string.h>
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
    
    U4MODE = 0x0000;
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
    while(U4STAbits.UTXBF==1);  // wait while the buffer is full
    U4TXREG=c;
}
/****************************************************************/
char utils_uart_getU4(void)
{
    while(!U4STAbits.URXDA);    //wait for a new char to arrive
    utils_uart_putU4_string("RICEVUTO");
    char ch = U4RXREG;          //read char from receive buffer
    return ch; 
}
/****************************************************************/
void utils_uart_putU4_string(const char* buffer)
{
    size_t size = strlen(buffer);
    while(size > 0) {
        utils_uart_putU4(*buffer);
        buffer++;
        size--;
    }
    
    while( !U4STAbits.TRMT); // wait for last trasmission to finish
}

int utils_uart_getU4_string(char* buffer, int max_sz)
{
    int i=0;
    char ch;
    for(i=0; i<max_sz || ch == '\r'; i++) {
        ch = utils_uart_getU4();
        buffer[i] = ch;
    }
    return i;
}
