/* 
 * File:   main.c
 * Author: massi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <p32xxxx.h>

#include "../utils.X/utils_common.h"
#include "../utils.X/utils_timer.h"
#include "../utils.X/utils_lcd.h"

// Joint Test Action Group (JTAG) interface
#pragma config JTAGEN = OFF     // Disable JTAG
#pragma config FWDTEN = OFF     // Watchdog Timer Disabled

// ***********************************************
// CLOCK CONFIGURATION (Program Time: DEVCFGx bits)

// Clock Sorgenti
// - Oscillatori interni: FRC (8Mhz), LPRC (31.25KHz)
// - Oscillatori esterni; POSC (8MHz), SOSC (32.768KHz))

// Configurazione PLL: Phase Locked Loop , per aumentare la frequenza
#pragma config FPLLIDIV = DIV_2 // 1) PLL IN: Divide FRC before PLL 
#pragma config FPLLMUL = MUL_20 // 2) PLL Multiply x range [4MHz <= IN < 5MHz]
#pragma config FPLLODIV = DIV_2 // 3) PLL OUT: Divide After PLL
// FRCPLL = FRC : FPLLIDIV * FPLLMUL : FPLLODIV = 8Mhz/2*20/2=40Mhz

// FNOSC Mutex: FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRCDIV16, FRCDIV 
//       In base alla selezione avro' un SYS Clock (e un per bus clk) diverso
#pragma config FNOSC = FRCPLL // FRCPLL: Internal Fast RC oscillator

// FPBDIV Postscaler: Peripheral Bus Clock Divisor
//#pragma config FPBDIV = DIV_2 // Peripheral Bus Clock = SYS clock / 2 = 20Mhz
#pragma config FPBDIV = DIV_4 // Peripheral Bus Clock = SYS clock / 4 = 10Mhz

// POSC: Primary Oscillator, ha effetto sul USB Clock
#pragma config POSCMOD = XT   // HS, XT, EC
#pragma config FSOSCEN = OFF  // Disable Secondary oscillator
#pragma config OSCIOFNC = ON  // CLKO Enable Configuration bit
// ***********************************************

const unsigned int periph_bus_clock_hz = 10000000; // 10 Mhz

#define CS          LATFbits.LATF8      // Chip Select (RF8)
#define TCS         TRISFbits.TRISF8
#define SPI_SCK     TRISFbits.TRISF6
#define SPI_SI      TRISFbits.TRISF2
#define SPI_SO      TRISFbits.TRISF7

void initSPI1() {
    SPI_SCK = OUTPUT;       // micro OUT, slave IN
    SPI_SI = OUTPUT;        // MOSI: micro OUT, slave IN
    SPI_SO = INPUT;         // MISO: micro IN, slave OUT
    RPF2R = 0x08;           // RPF2R mapping SDO1 (1000)
    SDI1R = 0x0F;           // SDI1R mapping RF7 (1111)
    
    TCS = OUTPUT;
    CS = 1;                 // Chip Select, low
    SPI1CON = 0x8120;       // SPI on, 8bit master, CKE=1, CKP=0
    SPI1BRG = 15;           // PB=10Mhz, SPI(15)=312.5 Khz
}

int writeSPI1(int byte) {
    while(!SPI1STATbits.SPITBE);    // wait for TX buffer to be empty
    SPI1BUF = byte;                 // write to buffer
    while(!SPI1STATbits.SPIRBF);
    return (int)SPI1BUF;
}

int getFlashID() {
    CS = 0;
    writeSPI1(0xAB);
    writeSPI1(0);
    writeSPI1(0);
    writeSPI1(0);
    int id = writeSPI1(0);
    CS = 1;
    return id;
}

void main() {
    initSPI1();
    int id = getFlashID();
}