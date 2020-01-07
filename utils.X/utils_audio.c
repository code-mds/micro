#include <p32xxxx.h>
#include "utils_audio.h"
#include "utils_common.h"

const unsigned short rgSinSamples[] = {
    256,320,379,431,472,499,511,507,488,453,
    406,350,288,224,162,106, 59, 24,  5,  1,
    13, 40, 81,133,192
};

#define TMR_FREQ_SINE   48000 // 48 kHz
#define RGSIN_SIZE  (sizeof(rgSinSamples) / sizeof(rgSinSamples[0]))

int cntAudioBuf, idxAudioBuf;

void utils_audio_init(int periph_bus_clock_hz) {
    utils_audio_beep_stop();
    
    TRISBbits.TRISB14 = OUTPUT;    
    RPB14R = 0x0C; // 1100 = OC1
    // disable analog (set pins as digital)
    ANSELBbits.ANSB14 = DIGITAL;
    
    PR3 = (int)((float)((float)periph_bus_clock_hz/TMR_FREQ_SINE) + 0.5);               
    TMR3 = 0;
    T3CONbits.TCKPS = 0;     //1:1 prescale value
    T3CONbits.TGATE = 0;     //not gated input (the default)
    T3CONbits.TCS = 0;       //PCBLK input (the default)
    T3CONbits.ON = 0;        //turn on Timer3
 
    IPC3bits.T3IP = 3;      // interrupt priority
    IPC3bits.T3IS = 3;      // interrupt subpriority
    IEC0bits.T3IE = 1;      // enable Timer3 interrupt    
    IFS0bits.T3IF = 0;      // clear Timer3 interrupt flag
    
    OC1CONbits.ON = 0;       // Turn off OC1 while doing setup.
    OC1CONbits.OCM = 6;      // PWM mode on OC1; Fault pin is disabled
    OC1CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
    OC1CONbits.ON = 0;       // Start the OC1 module  
}

void utils_audio_beep_start() {
    T3CONbits.ON = 0;       // turn off Timer3
    OC1CONbits.ON = 0;      // Turn off OC1
    
    idxAudioBuf = 0;
    cntAudioBuf = RGSIN_SIZE;
        
    // load first value
    OC1RS = rgSinSamples[0]; 
    
    OC1CONbits.ON = 1;
    T3CONbits.ON = 1;
}

void utils_audio_beep_stop() {
    T3CONbits.ON = 0;       // turn off Timer3
    OC1CONbits.ON = 0;      // Turn off OC1
}

void __attribute__(( interrupt(ipl3auto), vector(_TIMER_3_VECTOR)))
audio_timer3_int_handler(void) {
    OC1RS = 4 * rgSinSamples[(++idxAudioBuf) % cntAudioBuf];
    IFS0bits.T3IF = 0;
}