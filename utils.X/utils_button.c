#include <p32xxxx.h>
#include "utils_button.h"

void utils_button_init_btn_c(boolean_t use_interrupt, int priority, 
        int sub_priority) {
    
    // Table 12-1 Data Sheet - INPUT PIN SELECTION
    TRISFbits.TRISF0 = INPUT;       // RF0 (Button C) configured as input
    // Enable interupt for Button C
    if(use_interrupt) {
        // External Interrupt 4 remap RF0 (Button C)
        IEC0bits.INT4IE = 0;
        INT4R = 0b0100; 
        
        IPC4bits.INT4IP = priority;
        IPC4bits.INT4IS = sub_priority;
        IFS0bits.INT4IF = 0;    // clear interrupt flag
        IEC0bits.INT4IE = 1;    // enable interrupt
    }
}
int utils_button_get_btn_c() {
    return PORTFbits.RF0;
}

// RB8 BTNR (Button Right) 
void utils_button_init_btn_r() {
    TRISBbits.TRISB8 = INPUT;
    ANSELBbits.ANSB8 = DIGITAL;
}
int utils_button_get_btn_r() {
    return PORTBbits.RB8;
}

// RB0 BTNL (Button Left) 
void utils_button_init_btn_l() {
    TRISBbits.TRISB0 = INPUT;  
    ANSELBbits.ANSB0 = DIGITAL;
}
int utils_button_get_btn_l() {
    return PORTBbits.RB0;
}

// RB1 BTNU (Button Up) 
void utils_button_init_btn_u() {
    TRISBbits.TRISB1 = INPUT;       
    ANSELBbits.ANSB1 = DIGITAL;
}
int utils_button_get_btn_u() {
    return PORTBbits.RB1;
}