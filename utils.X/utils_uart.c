#include <p32xxxx.h>
#include <string.h>
#include "utils_uart.h"

void utils_uart4_ConfigurePins();
void utils_uart4_ConfigureUart(int baud, int pbusClock);

void utils_uart4_init(int baud, int pbus_clock) {
    utils_uart4_ConfigurePins();
    utils_uart4_ConfigureUart(baud, pbus_clock);
}

void utils_uart4_ConfigurePins() {
    TRISFbits.TRISF12 = 0;  // TX digital OUTPUT
    RPF12R = 2; //0b0010;   // mapping U4TX to RPF12
    
    TRISFbits.TRISF13 = 1;  // RX digital INPUT
    U4RXR = 9; //0b1001;    // mapping RPF13 to U4RX
}

void utils_uart4_ConfigureUart(int baud, int pbus_clock) {
    U4MODEbits.ON = 0;      // UART OFF

    // UxMODEbits.PDSEL - defines parity and number of data bits
    // 0b11 -> [9 data bits, no parity], 0b10 -> [8 data bits, odd parity]
    // 0b01 -> [8 data bits, even parity], 0b00 -> [8 data bits, no parity]
    U4MODEbits.PDSEL = 0b00;

    // UxMODEbits.UEN - defines wheter to use only TX and RX, or also RTS and CTS
    // 0b00 -> [Only the UxTX and UxRX are used]
    // 0b10 -> [UxTX, UxRX, UxCTS, and UxRTS are used]
    U4MODEbits.UEN = 0b00;

    // UxMODEbits.STSEL - defines number of stop bits
    // 0 -> [1 stop bit], 1 -> [2 stop bits]
    U4MODEbits.STSEL = 0;

    U4MODEbits.SIDL = 0;
    U4MODEbits.IREN = 0;
    U4MODEbits.RTSMD = 0;
    U4MODEbits.WAKE = 0;
    U4MODEbits.LPBACK = 0;
    U4MODEbits.ABAUD = 0;
    U4MODEbits.RXINV = 0;
    U4MODEbits.BRGH = 0;
    // U4BRG port speed
    U4BRG  = (int)(((float)pbus_clock / (16*baud) -1) + 0.5);
    
    U4STAbits.UTXEN = 1;    // abilita trasmissione
    U4STAbits.URXEN = 1;    // abilita ricezione
    U4MODEbits.ON = 1;      // UART ON
}

int utils_uart4_putc(int c) {
    while(U4STAbits.UTXBF == 1);  // wait while the buffer is full
    U4TXREG=c;                    // write the character to the output channel
}
/****************************************************************/
char utils_uart4_getc(void) {
    while(!U4STAbits.URXDA);    //wait for a new char to arrive
    char ch = U4RXREG;          //read char from receive buffer
    return ch; 
}
/****************************************************************/
void utils_uart4_puts(const char* buffer) {
    size_t size = strlen(buffer);
    while(size > 0) {
        utils_uart4_putc(*buffer);
        buffer++;
        size--;
    }
    
    while( !U4STAbits.TRMT); // wait for last trasmission to finish
}

int utils_uart4_gets(char* buffer, int max_sz) {
    int i=0;
    char ch = 0;
    for(i=0; i<max_sz && ch != '\r'; i++) {
        ch = utils_uart4_getc();
        buffer[i] = ch;
    }
    buffer[i-1] = 0;
    return i;
}
