#include <p32xxxx.h>
#include "utils_button.h"

void utils_button_init_btn_c(boolean_t use_interrupt, int priority, 
        int sub_priority) {
    
    // Table 12-1 Data Sheet - INPUT PIN SELECTION
    TRISFbits.TRISF0 = INPUT;       // RF0 (Button C) configured as input

    // Enable interupt for Button C
    if(use_interrupt) {
        // External Interrupt 2 remap RF0 (Button C)
        INT2R = 0b0101; 
        INTCONbits.INT2EP = 1;
        
        IPC2bits.INT2IP = priority;
        IPC2bits.INT2IS = sub_priority;
        IFS0bits.INT2IF = 0;    // clear interrupt flag
        IEC0bits.INT2IE = 1;    // enable interrupt
    }
}

int utils_button_get_btn_c() {
    return PORTFbits.RF0;
}