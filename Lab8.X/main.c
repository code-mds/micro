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

void utils_ocr_init() {
    ANSELBbits.ANSB8 = DIGITAL;
    TRISBbits.TRISB8 = OUTPUT;
    RPB8R = 0b1011; // mapping OC5
    
    OC5CONbits.OCM = 0b110; //PWM mode without Fault pin
    double duty_cycle = 0.1;
    
    OC5R = OC5RS = (PR2 +1) * duty_cycle;
    TMR2 = 0;
    OC5CONbits.ON = 1;
}

void __attribute__(( interrupt(ipl7auto), vector(_TIMER_2_VECTOR)))
Timer2Handler(void) {
    // Do something
    IFS0bits.T2IF = 0;      // clear interrupt flag
}


void main() {
    utils_common_macro_enable_interrupts();
    
    // init uart
    utils_uart4_init(9600, periph_bus_clock_hz);
    utils_uart4_puts("uart ready\r\n");
    
    // init timer 2
    int period_ms = 10;
    utils_timer2_init(period_ms, periph_bus_clock_hz, TMx_DIV_256, 
            TRUE, INT_PRIORITY_7, INT_SUB_PRIORITY_0);
    
    // init ocr
    utils_ocr_init();
    
    // init lcd
    utils_lcd_init(periph_bus_clock_hz, TM1_DIV_256);
    utils_lcd_write_str("Serie 8");
    
    int upper_bound = (2.5 / period_ms) * (PR2 +1);
    int lower_bound = (0.5 / period_ms) * (PR2 +1);
    int step = 10;
    
    int i;
    
    utils_lcd_cmd(0x80 | 0x40);
    utils_lcd_write_str("Upper");
    for(i = lower_bound; i<upper_bound; i+=step) {
        OC5RS = i;
        utils_timer1_delay(200, periph_bus_clock_hz, TM1_DIV_256);
    }
    
    utils_lcd_cmd(0x80 | 0x40);
    utils_lcd_write_str("Lower ");
    for(i = upper_bound; i>lower_bound; i-=step) {
        OC5RS = i;
        utils_timer1_delay(200, periph_bus_clock_hz, TM1_DIV_256);
    }
    
    while(1) {
        char cmd = utils_uart4_getc();
        if(cmd == '+') {
                int newVal = OC5RS + step;
                if(newVal < upper_bound)
                    OC5RS = newVal;
                else {
                    OC5RS = upper_bound;
                    utils_uart4_puts("upper bound exceeded\r\n");
                }
        } else if(cmd == '-') {
                int newVal = OC5RS - step;
                if(newVal > lower_bound)
                    OC5RS = newVal;
                else {
                    OC5RS = lower_bound;
                    utils_uart4_puts("lower bound exceeded\r\n");
                }
        }
    }
}