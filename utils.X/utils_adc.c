#include <p32xxxx.h>
#include "utils_adc.h"
#include "utils_timer.h"

void utils_adc_init() {
    ANSELB = 0xFFFB; // PORTB = Digital, RB = analog
    AD1CON1 = 0;     // ends sampling, start converting
    AD1CHS = 0x0020000; // Connect RB2/AN2 as CH0 input
    AD1CSSL = 0;
    AD1CON3 = 0x0002;   // manual Sample, Tad = internal 6 TPB
    AD1CON2 = 0;
    AD1CON1SET = 0x8000; // turn on the ADC
}

int utils_adc_get_int(int delay_ms, int bus_freq, tm1_prescaler_t prescaler) {
    AD1CON1SET = 0x0002; // start sampling
    utils_timer1_delay(delay_ms, bus_freq, prescaler);
    AD1CON1CLR = 0x0002; // start converting
    
    while(!(AD1CON1 & 0x0001)); // conversion done?
    
    return ADC1BUF0;
}
