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
#include "../utils.X/utils_audio.h"
#include "../utils.X/utils_rgb.h"

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

typedef enum {
    verde,
    giallo,
    rosso,
    blu_config,
    off
} stato_sem_t;

const char* DATALOG_CMD = "datalog";
const unsigned int periph_bus_clock_hz = 10000000; // 10 Mhz
const int   MIN_TR = 3;
const int   MAX_TR = 10;

stato_sem_t _stato_sem = verde;
int         _TG = 3;
int         _TR = 5;
int         _counter = 0;
int         _richieste_attraversamento = 0;
char        _uart_command[30];
int         _new_char_pos = 0;
int         _new_word = FALSE;

void delay(int delay_ms) {
    utils_timer2_delay(delay_ms, periph_bus_clock_hz, TMx_DIV_256);
}

void __attribute__(( interrupt(ipl7auto), vector(_TIMER_1_VECTOR)))
timer1_handler(void) {
    _counter--;
    IFS0bits.T1IF = 0; // reset interrupt
}

void __attribute__(( interrupt(ipl6auto), vector(_UART_4_VECTOR)))
uart4_handler(void) {
    char ch = U4RXREG;
    if(ch == '\r' || ch == '\n') {
        _new_word = TRUE;
        ch = '\0';
    }
    _uart_command[_new_char_pos] = ch;
    _new_char_pos++;
    
	IFS2bits.U4RXIF = 0;    // clear interrupt flag
}

void rgb(stato_sem_t stato_sem) {
    switch(stato_sem) {
        case verde:
            utils_rgb_set(0, 255, 0); 
            break;
        case rosso:
            utils_rgb_set(255, 0, 0); 
            break;
        case blu_config:
            utils_rgb_set(0, 0, 255); 
            break;
        case giallo:
            utils_rgb_set(200, 200, 0); 
            break;
        default:
            utils_rgb_set(0, 0, 0); 
    }

    /*
    utils_led_set(verde, stato_sem == verde ? LED_ON : LED_OFF);
    utils_led_set(giallo, stato_sem == giallo ? LED_ON : LED_OFF);
    utils_led_set(rosso, stato_sem == rosso ? LED_ON : LED_OFF);
    utils_led_set(blu_config, stato_sem == blu_config ? LED_ON : LED_OFF); */
}

void sem_verde() {
    //utils_lcd_write_str("verde");
    _stato_sem = verde;
    rgb(_stato_sem);
}

void sem_giallo_beep() {
    _stato_sem = giallo;  
    utils_lcd_cmd(0x80 | 0x40);    //cursore inizio seconda riga
    utils_lcd_write_str("giallo");

    utils_audio_beep_start();
    rgb(_stato_sem);
    
    _counter = _TG;
    utils_timer1_init(1000, periph_bus_clock_hz, TM1_DIV_256, 
                TRUE, INT_PRIORITY_7, INT_SUB_PRIORITY_0);
    while(_counter > 0) {
        utils_lcd_write_int(_counter);
    }
    utils_timer1_stop();
    utils_audio_beep_stop();
}

void sem_rosso_lampeggiante() {
    _stato_sem = rosso;
    utils_lcd_cmd(0x80 | 0x40);    //cursore inizio seconda riga
    utils_lcd_write_str("rosso ");
    
    _counter = _TR;
    utils_timer1_init(1000, periph_bus_clock_hz, TM1_DIV_256, 
                TRUE, INT_PRIORITY_7, INT_SUB_PRIORITY_0);
    while(_counter > 0) {
        utils_lcd_write_int(_counter);
        if(_counter%2) {
            rgb(rosso);
        } else {
            rgb(off);
        }
    }
    utils_timer1_stop();
}

void sem_config() {
    utils_lcd_clr();
    utils_lcd_write_str("config");

    int counter = 0;
    _stato_sem = blu_config;
    while(!utils_button_get_btn_u()) {
        utils_lcd_cmd(0x80 | 0x40);    //cursore inizio seconda riga
        utils_lcd_write_str("set TR");
        
        const int max_adc_val = 1023;
        counter = MIN_TR + utils_adc_get_int(delay, 100) * 
                 (MAX_TR-MIN_TR) / max_adc_val;
        utils_lcd_write_int(counter);
    }
    
    _TR = counter;
    sem_verde();
    utils_lcd_clr();
}

void richiesta_sx(){
    _richieste_attraversamento++;
    utils_lcd_clr();
    utils_lcd_write_str("call Sx");
    sem_giallo_beep();
    sem_rosso_lampeggiante();
    sem_verde();
    utils_lcd_clr();
}

void richiesta_dx(){
    _richieste_attraversamento++;
    utils_lcd_clr();
    utils_lcd_write_str("call Dx");
    sem_giallo_beep();
    sem_rosso_lampeggiante();
    sem_verde();
    utils_lcd_clr();
}

void data_log() {
    _new_word = FALSE;
    _new_char_pos = 0;
    if(strncmp(_uart_command, DATALOG_CMD, strlen(DATALOG_CMD)) == 0) {
        memset(_uart_command, 30, 0x00);
        char buffer[50];
        memset(buffer, 50, 0x00);
        
        sprintf(buffer, "Tempo Rosso=%02d\r\n", _TR);
        utils_uart4_puts(buffer);
        
        sprintf(buffer, "Tempo Giallo=%02d\r\n", _TG);
        utils_uart4_puts(buffer);
        
        sprintf(buffer, "Richieste Attraversamento=%02d\r\n", _richieste_attraversamento);
        utils_uart4_puts(buffer);
    }
}

void main() {
    utils_common_macro_enable_interrupts();
    
    // init uart
    utils_uart4_init_interrupt(9600, periph_bus_clock_hz, TRUE, 6, 0);
    utils_uart4_puts("SEMAFORO PEDONALE: uart ready\r\n");
    utils_adc_init();
    utils_rgb_init(periph_bus_clock_hz);
    utils_audio_init(periph_bus_clock_hz);
    utils_lcd_init(delay);
    utils_led_init(); 
    
    utils_button_init_btn_c(FALSE, 0, 0);
    utils_button_init_btn_u();
    utils_button_init_btn_r();
    utils_button_init_btn_l();
    
    sem_verde();
    while(1) {
        
        if(_new_word) {
            data_log();
        }
        
        if(utils_button_get_btn_l()) {
            richiesta_sx();
        }
        
        if(utils_button_get_btn_r()) {
            richiesta_dx();
        }
        
        if(utils_button_get_btn_c()) {
            sem_config();
        }
    }
}