#include <p32xxxx.h>
#include "utils_led.h"

void utils_led_init() {
    TRISA = 0x0000;
    LATA = 0x0000;
}

int utils_led_get(int idx) {
    int val = 0;
    switch(idx) {
        case 0:
            // Setto il singolo bit della Port A a 1
            val = LATAbits.LATA0;  //A0 corrisponde al LED0
            break;               
        case 1:
            val = LATAbits.LATA1;
            break;
        case 2:
            val = LATAbits.LATA2;
            break;
        case 3:
            val = LATAbits.LATA3;
            break;
        case 4:
            val = LATAbits.LATA4;
            break;
        case 5:
            val = LATAbits.LATA5;
            break;
        case 6:
            val = LATAbits.LATA6;
            break;
        case 7:
            val = LATAbits.LATA7; //A7 corrisponde al LED7
            break;
    }
    return val;
}

void utils_led_set(int idx, int value) {
    switch(idx) {
        case 0:
            // Setto il singolo bit della Port A a 1
            LATAbits.LATA0 = value;  //A0 corrisponde al LED0
            break;               
        case 1:
            LATAbits.LATA1 = value;
            break;
        case 2:
            LATAbits.LATA2 = value;
            break;
        case 3:
            LATAbits.LATA3 = value;
            break;
        case 4:
            LATAbits.LATA4 = value;
            break;
        case 5:
            LATAbits.LATA5 = value;
            break;
        case 6:
            LATAbits.LATA6 = value;
            break;
        case 7:
            LATAbits.LATA7 = value; //A7 corrisponde al LED7
            break;
    }
}

