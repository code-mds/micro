#include <p32xxxx.h>
#include <string.h>
#include "utils_uart.h"
#include "utils_common.h"

void utils_uart4_config_pins();
void utils_uart4_config_uart(int baud, int pbusClock);

void utils_uart4_init(int baud, int pbus_clock) {
    utils_uart4_init_interrupt(baud, pbus_clock, INTERRUPT_OFF, 0, 0);
}

void utils_uart4_init_interrupt(int baud, int pbus_clock, 
    use_interrupt_t use_interrupt, int priority, int sub_priority) {
    utils_uart4_config_pins();
    utils_uart4_config_uart(baud, pbus_clock);
    if(use_interrupt == INTERRUPT_ON) {
        // Micro Data Sheet - Tabella 7.2 INTERRUPT REGISTER MAP
        IPC9bits.U4IP = priority;  
        IPC9bits.U4IS = sub_priority;

        IFS2bits.U4RXIF = 0;    //Clear the Uart4 interrupt flag.
        IEC2bits.U4RXIE = 1;    // enable RX interrupt
    }
}

void utils_uart4_config_pins() {
    // Configure PINS
    // RP<n>:   Remappable Peripheral Port (Digital only) 
    //          Serial Comm, Timer, Interrupt on change
    
    // Table 12-2 Data Sheet - OUTPUT PIN SELECTION
    TRISFbits.TRISF12 = 0;  // TX digital OUTPUT
    RPF12R = 0b0010;        // mapping U4TX (0010) to RPF12 

    // Table 12-1 Data Sheet - INPUT PIN SELECTION
    TRISFbits.TRISF13 = 1;  // RX digital INPUT
    U4RXR = 0b1001;         // mapping RPF13 (1001) to U4RX
}

void utils_uart4_config_uart(int baud, int periph_bus_clock_hz) {
    U4MODEbits.ON = 0;      // UART OFF
    U4MODEbits.PDSEL = 0b00;// PDSEL - defines parity and number of data bits
                            // 0b11 -> [9 data bits, no parity]
                            // 0b10 -> [8 data bits, odd parity]
                            // 0b01 -> [8 data bits, even parity]
                            // 0b00 -> [8 data bits, no parity]

    U4MODEbits.UEN = 0b00;  // UEN: defines if use only TX and RX, or also RTS and CTS
                            // 0b00 -> [Only the UxTX and UxRX are used]
                            // 0b10 -> [UxTX, UxRX, UxCTS, and UxRTS are used]

    U4MODEbits.STSEL = 0;   // STSEL: defines number of stop bits
                            // 0 -> [1 stop bit], 1 -> [2 stop bits]
    
    U4MODEbits.SIDL = 0;    // SIDL: Stop in Idle Mode bit
    U4MODEbits.IREN = 0;    // IREN: IrDA Encoder and Decoder Enable bit
    U4MODEbits.RTSMD = 0;   // RTSMD: Mode Selection for UxRTS Pin bit
    U4MODEbits.WAKE = 0;    // WAKE: Enable Wake-up on Start bit Detect During Sleep Mode bit
    U4MODEbits.LPBACK = 0;  // LPBACK: UARTx Loopback Mode Select bit
    U4MODEbits.ABAUD = 0;   // ABAUD: Auto-Baud Enable bit
    U4MODEbits.RXINV = 0;   // RXINV: Receive Polarity Inversion bit
    U4MODEbits.BRGH = 0;    // BRGH: High Baud Rate Enable bit
                            // 1 = High-Speed mode ? 4x baud clock enabled
                            // 0 = Standard Speed mode ? 16x baud clock enabled
    // U4BRG port speed
    U4BRG  = (int)(((float)periph_bus_clock_hz / (16*baud) -1) + 0.5);
    
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
