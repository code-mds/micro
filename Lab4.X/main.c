/* 
 * File:   main.c
 * Author: massi
 * Title: Lab2
 * Created on 30. settembre 2019, 19:30
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

/* Pragma definitio for clock configuration */
// Device Config Bits in DEVCFG1 :
#pragma config FNOSC = FRCPLL // XTPLL, HSPLL, ECPLL, FRCPLL in FNOXC mux
                              // FRCPLL: Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FSOSCEN = OFF  // Disable Secondary oscillator
#pragma config POSCMOD = XT   // external crystal / resonator oscillator modes
#pragma config OSCIOFNC = ON  // CLKO Enable Configuration bit
#pragma config FPBDIV = DIV_2 // Peripheral Bus Clock Divisor

// Device Config Bits in DEVCFG2 :
#pragma config FPLLIDIV = DIV_2 // PLL Input Divider: Divide FRC before PLL 
#pragma config FPLLMUL = MUL_20 // PLL Multiply
#pragma config FPLLODIV = DIV_2 // PLL Output Divider: Divide After PLL

// disable JTAG
#pragma config JTAGEN = OFF     // Disable JTAG
#pragma config FWDTEN = OFF     // Watchdog Timer Disabled


const unsigned int baud = 9600;
const unsigned int pbus_clock = 20000000;
const unsigned int tm_period = 500;
//PR2 = 500 / ((1/20000000) * 1000 * 256) = 500 / 0.0128  = 39063

unsigned int timer_elapsed = 0;

void main() {
    utils_uart4_init(baud, pbus_clock);
    utils_uart4_puts("uart ready\r\n");
    
    utils_led_init();
    utils_uart4_puts("led ready\r\n");
    
    utils_timer2_init(
            tm_period, pbus_clock, TMx_DIV_256, 
            INTERRUPT_ON, 6, 0);
    utils_uart4_puts("timer ready\r\n");
        

    utils_common_macro_enable_interrupts();
    
    while(1) {
        if(timer_elapsed) {
            timer_elapsed = 0;
            utils_uart4_puts("timer interrupt xx\r\n");
        }
    }
}

void __attribute__(( interrupt(ipl6auto), vector(_TIMER_2_VECTOR)))
timer2_int_handler(void) {
    timer_elapsed = 1;
    IFS0bits.T2IF = 0; // reset interrupt
}