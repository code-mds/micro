#include <p32xxxx.h>
#include "utils_rgb.h"
#include "utils_common.h"

// RGBLED
#define tris_LED8_R         TRISDbits.TRISD2
#define rp_LED8_R           RPD2R
#define lat_LED8_R          LATDbits.LATD2
#define ansel_LED8_R        ANSELDbits.ANSD2

#define tris_LED8_G         TRISDbits.TRISD12
#define rp_LED8_G           RPD12R
#define lat_LED8_G          LATDbits.LATD12


#define tris_LED8_B         TRISDbits.TRISD3
#define rp_LED8_B           RPD3R
#define lat_LED8_B          LATDbits.LATD3
#define ansel_LED8_B        ANSELDbits.ANSD3

// Timer period in seconds
#define TMR_TIME    0.0003 // 300 us for each tick

// global variables to store R, G, B color values
volatile unsigned char bColR, bColG, bColB;

void __attribute__(( interrupt(ipl2auto), vector(_TIMER_5_VECTOR)))
rgb_timer5_int_handler(void) {
   static unsigned short sAccR = 0, sAccG = 0, sAccB = 0;
    
    // add 8 bit color values over the accumulators
    sAccR += bColR;
    sAccG += bColG;
    sAccB += bColB;

    // take the 9'th bit (addition carry) as the PDM
    lat_LED8_R = (sAccR & 0x100) ? 1: 0;    
    lat_LED8_G = (sAccG & 0x100) ? 1: 0;
    lat_LED8_B = (sAccB & 0x100) ? 1: 0;
    
    // filter only 8 bits in the accumulator
    sAccR &= 0xFF;
    sAccG &= 0xFF;
    sAccB &= 0xFF;
    
    IFS0bits.T5IF = 0;     // clear interrupt flag
}

void utils_rgb_init(int periph_bus_clock_hz) {
    rp_LED8_R = 0;      // no remapable
    tris_LED8_R = OUTPUT;    
    ansel_LED8_R = DIGITAL;

    rp_LED8_G = 0;      // no remapable
    tris_LED8_G = OUTPUT;    

    rp_LED8_B = 0;      // no remapable
    tris_LED8_B = OUTPUT;    
    ansel_LED8_B = DIGITAL;

    PR5 = utils_timer_calc_pr_16bit(1, periph_bus_clock_hz, 256);
    TMR5 = 0;                           //    initialize count to 0
    T5CONbits.TCKPS = 3;                //    1:256 prescaler value
    T5CONbits.TGATE = 0;                //    not gated input (the default)
    T5CONbits.TCS = 0;                  //    PCBLK input (the default)
    IPC5bits.T5IP = 2;                  //    INT step 4: priority
    IPC5bits.T5IS = 0;                  //    subpriority
    IFS0bits.T5IF = 0;                  //    clear interrupt flag
    IEC0bits.T5IE = 1;                  //    enable interrupt
    T5CONbits.ON = 1;                   //    turn on Timer5
    
    lat_LED8_R = 0;
    lat_LED8_G = 0;
    lat_LED8_B = 0;
}

void utils_rgb_set(int r, int g, int b){
    bColR = r;
    bColG = g;
    bColB = b;
}

