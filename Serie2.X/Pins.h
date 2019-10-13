/* 
 * File:   Pins.h
 * Author: Christian Tavilla
 *
 * Created on 13. ottobre 2019, 09:00
 */


// macros for switches button read
#define SW0 PORTFbits.RF3
#define SW1 PORTFbits.RF5
#define SW2 PORTFbits.RF4
#define SW3 PORTDbits.RD15
#define SW4 PORTDbits.RD14
#define SW5 PORTBbits.RB11
#define SW6 PORTBbits.RB10
#define SW7 PORTBbits.RB9

// macros for Led write
#define LED0 LATAbits.LATA0
#define LED1 LATAbits.LATA1
#define LED2 LATAbits.LATA2
#define LED3 LATAbits.LATA3
#define LED4 LATAbits.LATA4
#define LED5 LATAbits.LATA5
#define LED6 LATAbits.LATA6
#define LED7 LATAbits.LATA7

void portInit(void);
void TogglePort(void);