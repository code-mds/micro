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

#include "../utils.X/utils_uart.h"
#include "../utils.X/utils_led.h"
#include "../utils.X/utils_switch.h"
#include "../utils.X/utils_timer.h"


/* Pragma definitio for clock configuration */
// Device Config Bits in DEVCFG1 :
# pragma config FNOSC = FRCPLL // Select XTPLL , HSPLL , ECPLL , FRCPLL in FNOXC mux
# pragma config FSOSCEN = OFF // Disable Secondary oscillator
# pragma config POSCMOD = XT // external crystal / resonator oscillator modes
# pragma config OSCIOFNC = ON // CLKO Enable Configuration bit
# pragma config FPBDIV = DIV_2 // Peripheral Bus Clock Divisor
// Device Config Bits in DEVCFG2 :
# pragma config FPLLIDIV = DIV_2 // PLL Input Divider
# pragma config FPLLMUL = MUL_20 // PLL Multiplier
# pragma config FPLLODIV = DIV_2 // PLL Output Divider
// disable JTAG
# pragma config JTAGEN = OFF
# pragma config FWDTEN = OFF

unsigned int PbusClock = 20000000;

void es1();
void es2();

void main() {
    utils_led_init();
    
    int tm2_period = 500;
    int tm2_prescaler = 0b111;
    //PR2 = 500 / ((1/20000000) * 1000 * 256) = 500 / 0.0128  = 39063
    utils_timer2_init(tm2_period, PbusClock, tm2_prescaler);
    
    while(1) {
        es1();
        es2();
    }
}

void es1() {
    while(TMR2 < (PR2-1)); // wait timer
    utils_led_toggle(0);
    TMR2 = 0;
}