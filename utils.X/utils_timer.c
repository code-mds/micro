#include <p32xxxx.h>
#include "utils_timer.h"

// Timer 1 prescaler 0..3: 1,8,64,256
int tm1_prescaler_vals[] = {1, 8, 64, 256};

// Timer 2,3,4,5 Prescaler 0..7: 1,2,4,8,16,32,64,256
int tmx_prescaler_vals[] = {1, 2, 4, 8, 16, 32, 64, 256};

void utils_timer1_init(
        int period_ms, int bus_freq, tm1_prescaler_t prescaler, 
        tm_use_interrupt_t use_interrupt, int priority, int sub_priority) {
    T1CONbits.ON = 0;
    T1CONbits.TCKPS = prescaler; 
    TMR1 = 0;
    PR1 = calc_pr(period_ms, bus_freq, tm1_prescaler_vals[prescaler]); 
    
    IPC1bits.T1IP = priority;
    IPC1bits.T1IS = sub_priority;
    IEC0bits.T1IE = use_interrupt;
            
    T1CONbits.ON = 1;
}
    
void utils_timer2_init(
        int period_ms, int bus_freq, tmx_prescaler_t prescaler, 
        tm_use_interrupt_t use_interrupt, int priority, int sub_priority) {
    T2CONbits.ON = 0;
    T2CONbits.TCKPS = prescaler;
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    TMR2 = 0;
    PR2 = calc_pr(period_ms, bus_freq, tmx_prescaler_vals[prescaler]);
    
    IPC2bits.T2IP = priority;
    IPC2bits.T2IS = sub_priority;
    IEC0bits.T2IE = use_interrupt;
    
    T2CONbits.ON = 1;
}

// Calc Period Registration
int calc_pr(int period_ms, int bus_freq, int prescaler_val) {
   return  period_ms / ( (1.0/bus_freq) * 1000 * prescaler_val);
}
