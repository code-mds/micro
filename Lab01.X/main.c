/* 
 * File:   main.c
 * Author: massi
 * Title: Lab1
 * Created on 20. settembre 2019, 10:40
 */

#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>


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

// simulo delay 1 sec
void delay() {
    int counter = DELAY;
    while(counter) {
        counter--;
    }
}

// ANSELx: definisco se ANALOGICO=1 o DIGITAL=0
// TRISx: definisco se INPUT=1 o OUTPUT=0
// PORTx: per leggere il valore dei bit del registro
// LATx: per scrivere bit nel registro
// CNCON: interrupt (change modification)
// ODCx: OpenDrain control (non approfondiamo)

void init_switch() {
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
}

void testSwitch() {
    LATA = 0x0000;  // Spengo tutti i LED
    init_switch();
    while(1) {
        LATAbits.LATA0 = PORTFbits.RF3; // accendo il led0 se switch 0 on
        LATAbits.LATA1 = PORTFbits.RF5; // accendo il led1 se switch 1 on
        LATAbits.LATA2 = PORTFbits.RF4; // accendo il led2 se switch 2 on
        LATAbits.LATA3 = PORTDbits.RD15; // accendo il led3 se switch 3 on
        LATAbits.LATA4 = PORTDbits.RD14; // accendo il led4 se switch 4 on
        LATAbits.LATA5 = PORTBbits.RB11; // accendo il led5 se switch 5 on
        LATAbits.LATA6 = PORTBbits.RB10; // accendo il led6 se switch 6 on
        LATAbits.LATA7 = PORTBbits.RB9; // accendo il led7 se switch 7 on
    }
}

void testLoopLeds() {
    int i;
    while(1) {
        LATA = 0x0000;  // Spengo tutti i LED
            
        for(i= 0; i<8; i++) {
            delay();
            
            switch(i) {
                case 0:
                    // Setto il singolo bit della Port A a 1
                    LATAbits.LATA0 = 1;  //A0 corrisponde al LED0
                    break;               
                case 1:
                    LATAbits.LATA1 = 1;
                    break;
                case 2:
                    LATAbits.LATA2 = 1;
                    break;
                case 3:
                    LATAbits.LATA3 = 1;
                    break;
                case 4:
                    LATAbits.LATA4 = 1;
                    break;
                case 5:
                    LATAbits.LATA5 = 1;
                    break;
                case 6:
                    LATAbits.LATA6 = 1;
                    break;
                case 7:
                    LATAbits.LATA7 = 1; //A0 corrisponde al LED7
                    break;
            }
        }        
    }
}


/*
 * Accende tutti gli 8 led, uno alla volta con un ritardo di 1 secondo
 * Quando sono tutti accesi li spegne e ricomincia
 */
void main() {    
    // set Port A come OUTPUT
    TRISA = 0x0000; 

    testSwitch();
    // testLoopLeds() 
}