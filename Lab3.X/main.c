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
#pragma config FNOSC = FRCPLL // Select XTPLL , HSPLL , ECPLL , FRCPLL in FNOXC mux
#pragma config FSOSCEN = OFF // Disable Secondary oscillator
#pragma config POSCMOD = XT // external crystal / resonator oscillator modes
#pragma config OSCIOFNC = ON // CLKO Enable Configuration bit
#pragma config FPBDIV = DIV_2 // Peripheral Bus Clock Divisor
// Device Config Bits in DEVCFG2 :
#pragma config FPLLIDIV = DIV_2 // PLL Input Divider
#pragma config FPLLMUL = MUL_20 // PLL Multiplier
#pragma config FPLLODIV = DIV_2 // PLL Output Divider
// disable JTAG
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF

const unsigned int baud = 9600;             // 9600 bit/s
const unsigned int pbus_clock = 20000000;   // 20 MHz
const unsigned int tm2_period = 500;        // 500 ms
const unsigned tmx_prescaler_t tm2_prescaler = TMx_DIV_256;
//PR2 = 500 / ((1/20000000) * 1000 * 256) = 39063

void es1();
void es2();
void turn_leds_on();
void turn_leds_off();

void main() {
    utils_uart4_init(baud, pbus_clock);
    utils_uart4_puts("uart ready\r\n");
    
    utils_led_init();
    utils_uart4_puts("led ready\r\n");
    
    utils_timer2_init(
            tm2_period, pbus_clock, tm2_prescaler, 
            TM_INTERRUPT_OFF, 6, 0);
    utils_uart4_puts("timer ready\r\n");
    
    //es1();
    es2();
}

void es1() {
    while(1) {
        while(TMR2 < (PR2-1)); // wait timer
        utils_led_toggle(0);
        TMR2 = 0;
    }
}

void es2() {
    utils_uart4_puts("** enter a command: ledon | ledoff **\r\n");
    char command[30];
    while(1) {
        memset(command, 0, 30);
        utils_uart4_gets(command, 30);
        utils_common_tolower(command);
        
        if(strcmp(command, "ledon") == 0) {
            turn_leds_on();
        } else if(strcmp(command, "ledoff") == 0) {
            turn_leds_off();
        } else {
            utils_uart4_puts("invalid command\r\n");
        }
    }    
}

void turn_leds_on() {
    utils_uart4_puts("Leds turning on\r\n");
    int idx = 0;
    for(idx=0; idx<8; idx++) {
        while(TMR2 < (PR2-1)); // wait timer
        utils_led_set(idx, LED_ON);
        TMR2 = 0;
    }
}

void turn_leds_off() {
    utils_uart4_puts("Leds turning off\r\n");
    int idx = 0;
    for(idx=7; idx>=0; idx--) {
        while(TMR2 < (PR2-1)); // wait timer
        utils_led_set(idx, LED_OFF);
        TMR2 = 0;
    }
}