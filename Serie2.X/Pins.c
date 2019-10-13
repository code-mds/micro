
/* 
 * File:   Pins.c
 * Author: Christian Tavilla
 *
 * Created on 13. ottobre 2019, 09:00 */

#include <p32xxxx.h>


void portInit()
{
//    /* set ddr register */      //DDR is Data-Direction-Register Set if port is input or output ('0' for output, '1' for input)

/* LED pins configuration */    
    TRISA = 0xBF00;     // pin RA<0-7> set as output
    LATA = 0x0000;      // RA<0-7> write '0'
    
/* Switch pins configuration as digital input*/
    TRISFbits.TRISF3 = 1;   //sw0
    TRISFbits.TRISF5 = 1;   //sw1
    TRISFbits.TRISF4 = 1;   //sw2
    
    TRISDbits.TRISD15 =1;   //sw3
    TRISDbits.TRISD14 =1;   //sw4
    
    TRISBbits.TRISB11 = 1;  //sw5
    TRISBbits.TRISB10 = 1;  //sw6
    TRISBbits.TRISB9 = 1;   //sw7
    
    /* RB9 RB10 and RB11 can be analog -> so set as digital in ANSELx SFR*/
    ANSELBbits.ANSB11 = 0;  // set RB11 as digital
    ANSELBbits.ANSB10 = 0;  // set RB10 as digital
    ANSELBbits.ANSB9 = 0;   // set RB9 as digital
}

void TogglePort()
{

    //LATDbits.LATD9 = LATDINV;   // invert bit pin
    //LATDbits.LATD0 = ~LATDbits.LATD0;
    LATAINV = 0x0001;
    
}

