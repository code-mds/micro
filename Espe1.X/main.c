/* 
 * File:   main.c
 * Author: massi
 *
 * Created on 2. novembre 2019, 10:35
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <p32xxxx.h>

#include "../utils.X/utils_common.h"
#include "../utils.X/utils_uart.h"
#include "../utils.X/utils_led.h"
#include "../utils.X/utils_switch.h"
#include "../utils.X/utils_timer.h"

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
#pragma config FPBDIV = DIV_2 // Peripheral Bus Clock = SYS clock / 2 = 20Mhz

// POSC: Primary Oscillator, ha effetto sul USB Clock
#pragma config POSCMOD = XT   // HS, XT, EC
#pragma config FSOSCEN = OFF  // Disable Secondary oscillator
#pragma config OSCIOFNC = ON  // CLKO Enable Configuration bit
// ***********************************************

const unsigned int periph_bus_clock_hz = 20000000; // 20 Mhz

void init_timer() {
    // Timer Values
    const unsigned int tm_period_ms = 200;
    // PR2 = 500 (ms) / ((1/20000000) (micro sec) * 1000 * 256 (scaler)) 
    // = 39062.5 > (2^16-1) (65535), serve un timer a 16 bit
    const unsigned int tm_priority = 1;
    const unsigned int tm_subpriority = 0;

    utils_timer2_init(
            tm_period_ms, periph_bus_clock_hz, TMx_DIV_256, 
            INTERRUPT_ON, tm_priority, tm_subpriority);
    utils_uart4_puts("timer ready\r\n");
}

void init_uart4() {
    const unsigned int baud = 9600;
    utils_uart4_init(baud, periph_bus_clock_hz);
    utils_uart4_puts("uart ready\r\n");
}

void init_leds() {
    utils_led_init();
    utils_uart4_puts("led ready\r\n");
}

int _timer_elapsed = 0;
void main() {
    utils_common_macro_enable_interrupts();

    init_uart4();
    utils_uart4_puts("******** Espe1 ********\r\n");
    init_leds();
    init_timer();
    utils_uart4_puts("***********************\r\n"); 
            
    while(1) {  
        if(_timer_elapsed) {
            utils_uart4_puts(".");
            _timer_elapsed = 0;
        }
    }
}

void __attribute__(( interrupt(ipl1auto), vector(_TIMER_2_VECTOR)))
timer2_int_handler(void) {
    _timer_elapsed = 1;
    IFS0bits.T2IF = 0; // reset interrupt
}
