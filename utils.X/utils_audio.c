#include <p32xxxx.h>
#include "utils_audio.h"
#include "utils_common.h"

// https://daycounter.com/Calculators/Sine-Generator-Calculator2.phtml
const unsigned short rgSinSamples[] = {
5000,5245,5490,5734,5975,6215,6451,6684,6913,7138,7357,7571,7778,7978,8172,8358,
8536,8705,8865,9016,9157,9289,9410,9520,9619,9708,9785,9850,9904,9946,9976,9994,
10000,9994,9976,9946,9904,9850,9785,9708,9619,9520,9410,9289,9157,9016,8865,8705,
8536,8358,8172,7978,7778,7571,7357,7138,6913,6684,6451,6215,5975,5734,5490,5245,
5000,4755,4510,4266,4025,3785,3549,3316,3087,2862,2643,2429,2222,2022,1828,1642,
1464,1295,1135,984,843,711,590,480,381,292,215,150,96,54,24,6,
0,6,24,54,96,150,215,292,381,480,590,711,843,984,1135,1295,
1464,1642,1828,2022,2222,2429,2643,2862,3087,3316,3549,3785,4025,4266,4510,4755
};


#define TMR_FREQ_SINE   1000 // Hz
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
    //IEC0bits.T3IE = 1;      // enable Timer3 interrupt    
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
    //OC1RS = rgSinSamples[0];
    OC1RS = (PR3+1)*0.7;
    OC1R = OC1RS;
    
    OC1CONbits.ON = 1;
    T3CONbits.ON = 1;
}

void utils_audio_beep_stop() {
    T3CONbits.ON = 0;       // turn off Timer3
    OC1CONbits.ON = 0;      // Turn off OC1
}


void __attribute__(( interrupt(ipl3auto), vector(_TIMER_3_VECTOR)))
audio_timer3_int_handler(void) {
    OC1RS = rgSinSamples[(++idxAudioBuf) % cntAudioBuf];
    IFS0bits.T3IF = 0;
}
