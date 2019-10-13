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

void delay();
void es1_check_led_uart();
void es2_control_led_uart();

void main() {
    
    // uart configuration variables
    unsigned int baudRate = 9600;
    utils_uart_ConfigurePins();
    utils_uart_ConfigureUart(baudRate);
    utils_uart_putU4_string("uart ready\r\n");

    utils_led_init();
    utils_uart_putU4_string("led ready\r\n");
    
    utils_switch_init();
    utils_uart_putU4_string("switch ready\r\n");
    
 
    //es1_check_led_uart();
    es2_control_led_uart();
}

void delay() {    
    int counter = DELAY;
    while(counter) {
        counter--;
    }   
}

void es1_check_led_uart() {
    int i;
    while(1) {
        delay();
        // move the cursor to the home position (upper left hand of the screen)
        // without deleting any of the current information on the screen.
        char header[30];
        sprintf(header, "%c[H** Switch/Led Status **\r\n", 0x1B);
            
        utils_uart_putU4_string(header);
        for(i=0; i<8; i++) {
            int val = utils_switch_get(i);
            utils_led_set(i, val);
            
            char msg[30];
            sprintf(msg, "LED%d %s\r\n", i, 
                    val?"ON ":"OFF");

            utils_uart_putU4_string(msg);            
        }
    }       
}

void es2_control_led_uart() {
    utils_uart_putU4_string("** enter a command, e.g.: 'LED0 ON' **\r\n");
        
    char msg[30];
    while(1) {
        memset(msg, 0, 30);
        utils_uart_getU4_string(msg, 30);
        
        int i;
        for(i = 0; msg[i]; i++){
            msg[i] = tolower(msg[i]);
        }
        
        //LED0 ON
        char *led_name = strstr(msg, "led");
        char *led_on = strstr(msg, "on");
        if(led_name == NULL) {
            sprintf(msg, "KO, istruzione non valida\r\n");
        } else{
            int idx = led_name[3] - '0';
            if(idx >= 0 && idx <=7) {
                int op = led_on != NULL;
                utils_led_set(idx, op);
                sprintf(msg, "OK, LED%d %s\r\n", idx, 
                        op?"acceso":"spento");
            } else {
                sprintf(msg, "KO, led %d invalido \r\n" ,idx);
            }
        }
        
        utils_uart_putU4_string(msg);
    }
}
