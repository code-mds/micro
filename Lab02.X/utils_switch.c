#include <p32xxxx.h>
#include "utils_switch.h"

void utils_switch_init() {
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

int utils_switch_get(int idx) {
    int val = 0;
    switch(idx) {
        case 0:
            val = PORTFbits.RF3; // switch 0
            break;           
        case 1:
            val = PORTFbits.RF5; // switch 1
            break;
        case 2:
            val = PORTFbits.RF4; 
            break;
        case 3:
            val = PORTDbits.RD15;
            break;
        case 4:
            val = PORTDbits.RD14;
            break;
        case 5:
            val = PORTBbits.RB11; 
            break;
        case 6:
            val = PORTBbits.RB10; 
            break;
        case 7:
            val = PORTBbits.RB9; // switch 7 
            break;
    }
    return val;
}