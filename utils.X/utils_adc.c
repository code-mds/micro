#include <p32xxxx.h>
#include "utils_adc.h"
#include "utils_timer.h"

void utils_adc_init() {
    ANSELB = 0xFFFB; // PORTB = Digital, RB = analog
    AD1CHS = 0x0020000; // Connect RB2/AN2 as CH0 input

    AD1CON1 = 0;     // ends sampling, start converting, Integer 16-bit
    AD1CSSL = 0;
    AD1CON3 = 0x0002;   // manual Sample, TAD = internal 6 TPB
    AD1CON2 = 0;

    AD1CON1bits.ON = 1;
}

int utils_adc_get_int(void(*delay)(int), int delay_ms) {
    unsigned int adc_val = 0;
    
    AD1CON1SET = 0x0002; // start sampling
    delay(delay_ms);
    AD1CON1CLR = 0x0002; // start converting
    while(!(AD1CON1 & 0x0001)); // conversion done?

    adc_val = ADC1BUF0;
    return adc_val;
}
