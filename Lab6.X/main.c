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

void _general_exception_handler(unsigned cause, unsigned status) {    
}

void main() {
    tm1_prescaler_t prescaler = TM1_DIV_256;
    utils_timer1_delay(1000, periph_bus_clock_hz, prescaler);

    utils_lcd_init(periph_bus_clock_hz, prescaler);
    utils_lcd_write_str("Massimo");

    int counter = 0;
    char buffer[10];
    memset(buffer, 0, 10);

    while(1) {
        // 0x80	// set DDRAM position command
        utils_lcd_cmd(0x80 | 0x40);    //cursore inizio seconda riga
        utils_lcd_write_str("Serie6");
        utils_lcd_cmd(0x80 | 0x40 | 0x07);    //cursore inizio seconda riga

        sprintf(buffer, "%d", counter);
        utils_lcd_write_str(buffer);
        counter++;
        utils_timer1_delay(500, periph_bus_clock_hz, prescaler);
        utils_timer1_delay(500, periph_bus_clock_hz, prescaler);
    }
}