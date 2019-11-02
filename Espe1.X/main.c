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
const unsigned int tm_priority = 6;
const unsigned int tm_subpriority = 0;
const tm1_prescaler_t tm_prescaler = TM1_DIV_256;
const tm_use_interrupt_t tm_use_interrupt = TM_INTERRUPT_ON;

//PR2 = 500 / ((1/20000000) * 1000 * 256) = 500 / 0.0128  = 39063
unsigned int timer_elapsed = 0;

int main(int argc, char** argv) {
    utils_uart_init(baud, pbus_clock);
    utils_uart_putU4_string("uart ready\r\n");
    
    utils_led_init();
    utils_uart_putU4_string("led ready\r\n");
    
    utils_timer1_init(
            tm_period, pbus_clock, tm_prescaler, 
            tm_use_interrupt, tm_priority, tm_subpriority);
    utils_uart_putU4_string("timer ready\r\n");
        

    utils_common_macro_enable_interrupts();
    
    while(1) {
        if(timer_elapsed) {
            timer_elapsed = 0;
            utils_uart_putU4_string("timer1 interrupt xx\r\n");
        }
    }
}

void __attribute__(( interrupt(ipl6), vector(_TIMER_1_VECTOR)))
timer1_int_handler(void) {
    timer_elapsed = 1;
    IFS0bits.T1IF = 0; // reset interrupt
}
