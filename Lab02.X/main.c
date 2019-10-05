/* 
 * File:   main.c
 * Author: massi
 * Title: Lab2
 * Created on 30. settembre 2019, 19:30
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include "Uart.h"

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

#define DELAY 1000000 // 1 second

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

    UART_ConfigurePins();
    UART_ConfigureUart(baudRate);
    
    while(1) {
        delay();
        putU4string("CIAO\r");
    }    
}

