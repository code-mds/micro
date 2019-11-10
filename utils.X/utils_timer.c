#include <p32xxxx.h>
#include "utils_timer.h"

// Timer 1 prescaler 0..3: 1,8,64,256
int tm1_prescaler_vals[] = {1, 8, 64, 256};

// Timer 2,3,4,5 Prescaler 0..7: 1,2,4,8,16,32,64,256
int tmx_prescaler_vals[] = {1, 2, 4, 8, 16, 32, 64, 256};

// TIMER 1: 16 bit
// - use _TIMER_1_VECTOR for interrupt, need to reset IFS0bits.T1IF
void utils_timer1_init(
        int period_ms, int bus_freq, tm1_prescaler_t prescaler, 
        boolean_t use_interrupt, int priority, int sub_priority) {
    T1CON = 0x00;                   // STOP timer 1 and reset configuration 
    T1CONbits.TCKPS = prescaler;    // set prescaler
    TMR1 = 0;                       // reset counters to 0
    PR1 = calc_pr_16bit(period_ms, bus_freq, tm1_prescaler_vals[prescaler]); 
    
    // configure interrupt
    IPC1bits.T1IP = priority;
    IPC1bits.T1IS = sub_priority;
    IEC0bits.T1IE = use_interrupt;
            
    T1CONbits.ON = 1;
}

// TIMER 2: 16 bit    
// - use _TIMER_2_VECTOR for interrupt, need to reset IFS0bits.T2IF
void utils_timer2_init(
        int period_ms, int bus_freq, tmx_prescaler_t prescaler, 
        boolean_t use_interrupt, int priority, int sub_priority) {
    T2CON = 0x00;                   // STOP timer 2 and reset configuration            
    T2CONbits.TCKPS = prescaler;    // set prescaler
    TMR2 = 0;                       // reset counters to 0 
    PR2 = calc_pr_16bit(period_ms, bus_freq, tmx_prescaler_vals[prescaler]);
    
    // configure interrupt
    IEC0bits.T2IE = 0; // disable interrupt
    IFS0bits.T2IF = 0; //Interrupt flag put at zero
    IPC2bits.T2IP = priority;
    IPC2bits.T2IS = sub_priority;
    IEC0bits.T2IE = use_interrupt;
    
    T2CONbits.ON = 1; // turn ON the timer 
}

// TIMER 2/3: 32 bit    
// - use _TIMER_3_VECTOR for interrupt, need to reset IFS0bits.T3IF and T2IF
// the 32-bit timer is controlled by the even number timer control registers (e.g. Timer2) 
// When an interrupt event occurs, the odd number timer generates the event (e.g. Timer3)
void utils_timer23_init_32bit(
        int period_ms, int bus_freq, tmx_prescaler_t prescaler, 
        boolean_t use_interrupt, int priority, int sub_priority) {
    T2CON = 0x00; // STOP timer2 and reset configuration
    T3CON = 0x00; // STOP timer3 and reset configuration

    T2CONbits.TCKPS = prescaler;    // set prescaler
    T2CONbits.T32 = 1;              // set the timer to 32bit mode
    T2CONbits.TCS = 0;              // use pheriperal bus clock
    // reset counters to 0
    TMR2 = 0;   
    TMR3 = 0;
    
    // Load the period register PRx with the desired 32-bit match value
    PR2 = calc_pr(period_ms, bus_freq, tmx_prescaler_vals[prescaler]);
    
    // configure interrupt
    IEC0bits.T3IE = 0;
    IFS0bits.T3IF = 0; //Interrupt flag putted at zero
    IPC3bits.T3IP = priority;
    IPC3bits.T3IS = sub_priority;
    IEC0bits.T3IE = use_interrupt;
    
    T2CONbits.ON = 1; // turn ON the timer 
}

// Calc Period Registration
int calc_pr(int period_ms, int bus_freq, int prescaler_val) {
   int ms_to_micros = 1000;
   return  period_ms / ( (1.0/bus_freq) * ms_to_micros * prescaler_val);
}

int calc_pr_16bit(int period_ms, int bus_freq, int prescaler_val) {
    unsigned int pr = calc_pr(period_ms, bus_freq, prescaler_val);
    if(pr > 0xFFFF) {
        int wrong_config = 1;
    }
    return pr;
}
