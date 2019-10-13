#include <p32xxxx.h>
#include "utils_timer.h"

// prescaler 0..3: 1,8,64,256
void utils_timer1_init(int period_ms, int bus_freq, int prescaler_idx) {
    int tm1_prescaler_vals[] = {1, 8, 64, 256};
    T1CONbits.ON = 0;
    T1CONbits.TCKPS = prescaler_idx; 
    TMR1 = 0;
    PR1 = calc_pr(period_ms, bus_freq, tm1_prescaler_vals[prescaler_idx]); 
    T1CONbits.ON = 1;
}

// prescaler 0..7: 1,2,4,8,16,32,64,256
void utils_timer2_init(int period_ms, int bus_freq, int prescaler_idx) {
    int tm2_prescaler_vals[] = {1, 2, 4, 8, 16, 32, 64, 256};
    T2CONbits.ON = 0;
    T2CONbits.TCKPS = prescaler_idx;
    T2CONbits.T32 = 0;
    TMR2 = 0;
    PR2 = calc_pr(period_ms, bus_freq, tm2_prescaler_vals[prescaler_idx]);
    T2CONbits.ON = 1;
}

int calc_pr(int period_ms, int bus_freq, int prescaler_val) {
   return  period_ms / ( (1.0/bus_freq) * 1000 * prescaler_val);
}
