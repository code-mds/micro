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

/*
 * Accende tutti gli 8 led, uno alla volta con un ritardo di 1 secondo
 * Quando sono tutti accesi li spegne e ricomincia
 */
void main() {    
    // set Port A come OUTPUT
    // la porta A ha 12 pin
    TRISA = 0x0000; 
    
    int i;
    while(1) {
        LATA = 0x0000;  // Metto tutti i bit Port A a 0
            
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

