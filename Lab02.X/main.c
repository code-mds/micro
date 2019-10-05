/* 
 * File:   main.c
 * Author: massi
 * Title: Lab2
 * Created on 30. settembre 2019, 19:30
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include "utils_uart.h"
#include "utils_led.h"
#include "utils_switch.h"

/* Disable JTAG to use RA0 */   
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF

/* Device Config Bits in DEVCFG1: */
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = XT
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_2

/* Device Config Bits in DEVCFG2: */
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#define DELAY 500000 // 1 second

/*
 * 
 */
void delay() {
    int counter = DELAY;
    while(counter) {
        counter--;
    }   
}

void main() {
    // uart configuration variables
    unsigned int baudRate = 9600;
    // data to send

    utils_led_init();
    utils_switch_init();
    
    utils_uart_ConfigurePins();
    utils_uart_ConfigureUart(baudRate);
    
    int i;
    while(1) {
        delay();
        //utils_uart_putU4string("\x1b[2J");
        char header[30];
        sprintf(header, "%c[H** Switch/Led Status **\r\n", 0x1B);
            
        utils_uart_putU4string(header);
        for(i=0; i<8; i++) {
            int val = utils_switch_get(i);
            utils_led_set(i, val);
            
            char msg[30];
            sprintf(msg, "LED%d %s\r\n", i, 
                    val?"ON ":"OFF");
            
//            val = utils_led_get(i);
//            sprintf(msg, "OK, LED%d %s\r", i, 
//                   val?"acceso":"spento");

            utils_uart_putU4string(msg);            
        }
    }    
}

