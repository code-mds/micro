#include <p32xxxx.h>
#include <string.h>
#include "utils_uart.h"

void utils_uart_ConfigurePins();
void utils_uart_ConfigureUart(int baud, int pbusClock);

void utils_uart_init(int baud, int pbus_clock) {
    utils_uart_ConfigurePins();
    utils_uart_ConfigureUart(baud, pbus_clock);
}

void utils_uart_ConfigurePins() {
    TRISFbits.TRISF12 = 0;  // TX digital output
    RPF12R = 2; //0b0010;   // mapping U4TX to RPF12
    
    TRISFbits.TRISF13 = 1;  // RX digital output
    U4RXR = 9;//0b1001;     // mapping RPF13 to U4RX
}

void utils_uart_ConfigureUart(int baud, int pbus_clock) {
    U4MODEbits.ON = 0;
    U4MODEbits.SIDL = 0;
    U4MODEbits.IREN = 0;
    U4MODEbits.RTSMD = 0;
    U4MODEbits.UEN0 = 0;
    U4MODEbits.UEN1 = 0;
    U4MODEbits.WAKE = 0;
    U4MODEbits.LPBACK = 0;
    U4MODEbits.ABAUD = 0;
    U4MODEbits.RXINV = 0;
    U4MODEbits.PDSEL1 = 0; // nessuna parita
    U4MODEbits.PDSEL0 = 0;
    U4MODEbits.BRGH = 0;
    U4MODEbits.STSEL = 0; // 1 bit stop    
    U4BRG  = (int)(((float)pbus_clock / (16*baud) -1) + 0.5);
    
    U4STAbits.UTXEN = 1;    // abilita trasmissione
    U4STAbits.URXEN = 1;    // abilita ricezione
    U4MODEbits.ON = 1;      // UART ON
}

int utils_uart_putU4(int c)
{
    while(U4STAbits.UTXBF == 1);  // wait while the buffer is full
    U4TXREG=c;
}
/****************************************************************/
char utils_uart_getU4(void)
{
    while(!U4STAbits.URXDA);    //wait for a new char to arrive
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
    char ch = 0;
    for(i=0; i<max_sz && ch != '\r'; i++) {
        ch = utils_uart_getU4();
        buffer[i] = ch;
    }
    buffer[i-1] = 0;
    return i;
}
