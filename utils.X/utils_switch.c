#include <p32xxxx.h>
#include "utils_switch.h"

void utils_switch_init() {
    TRISFbits.TRISF3 = INPUT;       // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = INPUT;       // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = INPUT;       // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = INPUT;      // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = INPUT;      // RD14 (SW4) configured as input
    
    TRISBbits.TRISB11 = INPUT;      // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = DIGITAL;    // RB11 (SW5) disabled analog
    
    TRISBbits.TRISB10 = INPUT;      // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = DIGITAL;    // RB10 (SW6) disabled analog
    
    TRISBbits.TRISB9 = INPUT;       // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = DIGITAL;     // RB9 (SW7) disabled analog
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

// usare _CHANGE_NOTICE_VECTOR
void utils_switch_init_sw0_interrupt_cn(int priority, int subpriority) {
    // Port F pin RF3 (switch 0) Interrupt change notification (CN))

    // TABLE 12-1: INPUT PIN SELECTION 
    // Enable interupt for Switch 0
    TRISFbits.TRISF3 = INPUT;       // RF3 (SW0) configured as input
    CNCONFbits.ON = 1;
    CNENFbits.CNIEF3 = 1;   // enable change notification interrupt su pin RF3
    CNPUFbits.CNPUF3 = 0;   // enable pull-up
    IEC1bits.CNFIE = 1;     //enable interrupt su porta F
    IFS1bits.CNFIF = 0;     //reset flag su porta F
    IPC8bits.CNIP = priority;
    IPC8bits.CNIS = subpriority;
}