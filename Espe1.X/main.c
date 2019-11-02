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

// CLOCK CONFIGURATION (Program Time: DEVCFGx bits)
#pragma config FNOSC = FRCPLL // XTPLL, HSPLL, ECPLL, FRCPLL in FNOXC mux
                              // FRCPLL: Internal Fast RC oscillator (8 MHz)
// POSC: Primary Oscillator
#pragma config POSCMOD = XT   // HS, XT, EC
#pragma config FSOSCEN = OFF  // Disable Secondary oscillator
#pragma config OSCIOFNC = ON  // CLKO Enable Configuration bit
#pragma config FPBDIV = DIV_2 // Peripheral Bus Clock Divisor

// Configurazione PLL: Phase Locked Loop , per aumentare la frequenza
#pragma config FPLLIDIV = DIV_2 // 1) PLL IN: Divide FRC before PLL 
#pragma config FPLLMUL = MUL_20 // 2) PLL Multiply ( 4MHz <= IN < 5MHz )
#pragma config FPLLODIV = DIV_2 // 3) PLL OUT: Divide After PLL

const unsigned int baud = 9600;
const unsigned int pbus_clock_hz = 20000000; // 20 Mhz

// Timer Values
const unsigned int tm_period_ms = 500;
const tm1_prescaler_t tm_prescaler = TM1_DIV_256;
//PR2 = 500 (ms) / ((1/20000000) (micro sec) * 1000 * 256 (scaler)) = 39062.5
const tm_use_interrupt_t tm_use_interrupt = TM_INTERRUPT_ON;
const unsigned int tm_priority = 6;
const unsigned int tm_subpriority = 0;

unsigned int timer_elapsed = 0;

int main(int argc, char** argv) {
    utils_uart4_init(baud, pbus_clock_hz);
    utils_uart4_puts("uart ready\r\n");
    
    utils_led_init();
    utils_uart4_puts("led ready\r\n");
    
    utils_timer1_init(
            tm_period_ms, pbus_clock_hz, tm_prescaler, 
            tm_use_interrupt, tm_priority, tm_subpriority);
    utils_uart4_puts("timer ready\r\n");
        
    utils_common_macro_enable_interrupts();
    
    while(1) {
        if(timer_elapsed) {
            timer_elapsed = 0;
            utils_uart4_puts("timer1 interrupt xx\r\n");
        }
    }
}

void __attribute__(( interrupt(ipl6), vector(_TIMER_1_VECTOR)))
timer1_int_handler(void) {
    timer_elapsed = 1;
    IFS0bits.T1IF = 0; // reset interrupt
}
