/* 
 * File:   main.c
 * Author: massi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <p32xxxx.h>

#include "../utils.X/utils_common.h"
#include "../utils.X/utils_timer.h"
#include "../utils.X/utils_lcd.h"
#include "../utils.X/utils_led.h"

// Joint Test Action Group (JTAG) interface
#pragma config JTAGEN = OFF     // Disable JTAG
#pragma config FWDTEN = OFF     // Watchdog Timer Disabled

// ***********************************************
// CLOCK CONFIGURATION (Program Time: DEVCFGx bits)

// Clock Sorgenti
// - Oscillatori interni: FRC (8Mhz), LPRC (31.25KHz)
// - Oscillatori esterni; POSC (8MHz), SOSC (32.768KHz))

// Configurazione PLL: Phase Locked Loop , per aumentare la frequenza
#pragma config FPLLIDIV = DIV_2 // 1) PLL IN: Divide FRC before PLL 
#pragma config FPLLMUL = MUL_20 // 2) PLL Multiply x range [4MHz <= IN < 5MHz]
#pragma config FPLLODIV = DIV_2 // 3) PLL OUT: Divide After PLL
// FRCPLL = FRC : FPLLIDIV * FPLLMUL : FPLLODIV = 8Mhz/2*20/2=40Mhz

// FNOSC Mutex: FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRCDIV16, FRCDIV 
//       In base alla selezione avro' un SYS Clock (e un per bus clk) diverso
#pragma config FNOSC = FRCPLL // FRCPLL: Internal Fast RC oscillator

// FPBDIV Postscaler: Peripheral Bus Clock Divisor
//#pragma config FPBDIV = DIV_2 // Peripheral Bus Clock = SYS clock / 2 = 20Mhz
#pragma config FPBDIV = DIV_4 // Peripheral Bus Clock = SYS clock / 4 = 10Mhz

// POSC: Primary Oscillator, ha effetto sul USB Clock
#pragma config POSCMOD = XT   // HS, XT, EC
#pragma config FSOSCEN = OFF  // Disable Secondary oscillator
#pragma config OSCIOFNC = ON  // CLKO Enable Configuration bit
// ***********************************************

const unsigned int periph_bus_clock_hz = 10000000; // 10 Mhz
const int MIN_TR = 3;
const int MAX_TR = 10;

void delay(int delay_ms) {
    utils_timer2_delay(delay_ms, periph_bus_clock_hz, TMx_DIV_256);
}

typedef enum {
    verde,
    giallo,
    rosso,
    blu_config,
    off
} stato_sem_t;

stato_sem_t stato_sem = verde;
int TG = 3;
int TR = 5;
int lampeggio = 0;
int richieste_attraversamento = 0;

void __attribute__(( interrupt(ipl7auto), vector(_TIMER_1_VECTOR)))
timer1_int_handler(void) {
    lampeggio--;
    IFS0bits.T1IF = 0; // reset interrupt
}

void rgb(stato_sem_t stato_sem) {
    utils_led_set(verde, stato_sem == verde ? LED_ON : LED_OFF);
    utils_led_set(giallo, stato_sem == giallo ? LED_ON : LED_OFF);
    utils_led_set(rosso, stato_sem == rosso ? LED_ON : LED_OFF);
    utils_led_set(blu_config, stato_sem == blu_config ? LED_ON : LED_OFF);
}

void sem_verde() {
    stato_sem = verde;
    rgb(verde);
}

void sem_giallo_beep() {
    stato_sem = giallo;  
    rgb(giallo);
    int secs;
    for(secs = TG; secs>0; secs--)
        delay(1000);
}

void sem_rosso_lampeggiante() {
    stato_sem = rosso;
    
    utils_timer1_init(1000, periph_bus_clock_hz, TM1_DIV_256, 
                TRUE, INT_PRIORITY_7, INT_SUB_PRIORITY_0);
    lampeggio = TR;
    while(lampeggio > 0) {
        rgb(lampeggio%2 ? rosso : off);
    }
}

void sem_config() {
    int counter = 0;
    char buffer[10];
    memset(buffer, 0, 10);
 
    stato_sem = blu_config;
    while(!utils_button_get_btn_u()) {
        counter = utils_adc_get_int(100, periph_bus_clock_hz, TM1_DIV_256);
        sprintf(buffer, "%d", counter);
        utils_lcd_write_str(buffer);
    }
}

void main() {
    utils_common_macro_enable_interrupts();
    
    // init uart
    utils_uart4_init(9600, periph_bus_clock_hz);
    utils_uart4_puts("SEMAFORO PEDONALE: uart ready\r\n");
    
    // init lcd
    utils_lcd_init(delay);
    utils_adc_init();
    
    utils_led_init(); //TODO: modify with RGB
    
    utils_button_init_btn_c(FALSE, 0, 0);
    utils_button_init_btn_u();
    utils_button_init_btn_r();
    utils_button_init_btn_l();
    
    while(1) {
        
        if(utils_button_get_btn_l()) {
            richieste_attraversamento++;
            utils_lcd_write_str("call Sx");
            sem_giallo_beep();
            sem_rosso_lampeggiante();
            sem_verde();
            utils_lcd_clr();
        }
        
        if(utils_button_get_btn_r()) {
            richieste_attraversamento++;
            utils_lcd_write_str("call Dx");
            sem_giallo_beep();
            sem_rosso_lampeggiante();
            sem_verde();
            utils_lcd_clr();
        }
        
        if(utils_button_get_btn_c()) {
            utils_lcd_write_str("config");
            sem_config();
            sem_verde();
            utils_lcd_clr();
        }
    }
}