/* 
 * File:   main.c
 * Author: CTa
 * Title: Interrupt UART and Timer example for serie 4
 * Created on 13. ottobre 2019, 09:00 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <p32xxxx.h>
//#include <plib.h>     //uncomment to use the other interrupt configuration

#include "Uart.h"
#include "Pins.h"





/* Pragma definition for clock configuration */      

// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL      // Select XTPLL, HSPLL, ECPLL, FRCPLL in FNOXC mux
#pragma config FSOSCEN =	OFF     // Disable Secondary oscillator
#pragma config POSCMOD =	XT      // external crystal/resonator oscillator modes
#pragma config OSCIOFNC =	ON      // CLKO Enable Configuration bit 
#pragma config FPBDIV =     DIV_2   // Peripheral Bus Clock Divisor

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2   // PLL Input Divider 
#pragma config FPLLMUL =	MUL_20  // PLL Multiplier
#pragma config FPLLODIV =	DIV_2   // PLL Output Divider 

// disable JTAG
#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF

/* Global variables definition */
unsigned char strg[80]; // global because used by ISR
int j = 0;              // global because used by ISR
char flagRX = 0;        // global because used by ISR


/*
 * 
 */
void main(void) 
{
   
     
    portInit();           // gpio configuration
    UART_ConfigurePins();   // uart configuration
    UART_ConfigureUart(9600); // uart configuration
 
    putU4_string("\033[2J \r");     // clear terminal screen
   
    putU4_string("Uart4_init_completed_at_9600\r");
    while(1)
    {
        getU4_string(); // wait message from terminal
        
        if(flagRX == 1)     // flagRX is set by Interrup Service Routine
        {
            if(strcmp(strg, "led0 on")== 0)   // RX string comparation
            {
                LED0 = 1;
               putU4_string("ok, led0 on\r");
            }
            else if(strcmp(strg, "led0 off")== 0)   // RX string comparation
            {
                LED0 = 0;
               putU4_string("ok, led0 off\r");
            }
            else if(strcmp(strg, "led1 on")== 0)   // RX string comparation
            {
                LED1 = 1;
               putU4_string("ok, led1 on\r");
            }
            else if(strcmp(strg, "led1 off")== 0)   // RX string comparation
            {
                LED1 = 0;
               putU4_string("ok, led1 off\r");
            }
            else if(strcmp(strg, "led2 on")== 0)   // RX string comparation
            {
                LED2 = 1;
               putU4_string("ok, led2 on\r");
            }
            else if(strcmp(strg, "led2 off")== 0)   // RX string comparation
            {
                LED2 = 0;
               putU4_string("ok, led2 off\r");
            }
            else if(strcmp(strg, "led3 on")== 0)   // RX string comparation
            {
                LED3 = 1;
               putU4_string("ok, led3 on\r");
            }
            else if(strcmp(strg, "led3 off")== 0)   // RX string comparation
            {
                LED3 = 0;
               putU4_string("ok, led3 off\r");
            }
            else if(strcmp(strg, "led4 on")== 0)   // RX string comparation
            {
                LED4 = 1;
               putU4_string("ok, led4 on\r");
            }
            else if(strcmp(strg, "led4 off")== 0)   // RX string comparation
            {
                LED4 = 0;
               putU4_string("ok, led4 off\r");
            }
            else if(strcmp(strg, "led5 on")== 0)   // RX string comparation
            {
                LED5 = 1;
               putU4_string("ok, led5 on\r");
            }
            else if(strcmp(strg, "led5 off")== 0)   // RX string comparation
            {
                LED5 = 0;
               putU4_string("ok, led5 off\r");
            }
            else if(strcmp(strg, "led6 on")== 0)   // RX string comparation
            {
                LED6 = 1;
               putU4_string("ok, led6 on\r");
            }
            else if(strcmp(strg, "led6 off")== 0)   // RX string comparation
            {
                LED6 = 0;
               putU4_string("ok, led6 off\r");
            }
            else if(strcmp(strg, "led7 on")== 0)   // RX string comparation
            {
                LED7 = 1;
               putU4_string("ok, led7 on\r");
            }
            else if(strcmp(strg, "led7 off")== 0)   // RX string comparation
            {
                LED7 = 0;
               putU4_string("ok, led7 off\r");
            }
            else
            {
                putU4_string("Error command\r");
            }
            
            flagRX = 0; // flagRX reset, because only when next message arrives
        }
    }   // end while(1)
}   // end main