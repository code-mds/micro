/* 
 * File:   main.c
 * Author: massi
 *
 * Created on 2. novembre 2019, 10:35
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <p32xxxx.h>

#include "../utils.X/utils_common.h"
#include "../utils.X/utils_uart.h"
#include "../utils.X/utils_led.h"
#include "../utils.X/utils_switch.h"
#include "../utils.X/utils_timer.h"

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
#pragma config FPBDIV = DIV_2 // Peripheral Bus Clock = SYS clock / 2 = 20Mhz

// POSC: Primary Oscillator, ha effetto sul USB Clock
#pragma config POSCMOD = XT   // HS, XT, EC
#pragma config FSOSCEN = OFF  // Disable Secondary oscillator
#pragma config OSCIOFNC = ON  // CLKO Enable Configuration bit
// ***********************************************

const unsigned int periph_bus_clock_hz = 20000000; // 20 Mhz
const char* CMD_LED_ON = "ledon";
const char* CMD_LED_OFF = "ledoff";

void init_uart4() {
    const unsigned int baud = 9600;
    const unsigned int priority = 6;
    const unsigned int subpriority = 3;
    utils_uart4_init_interrupt(baud, periph_bus_clock_hz, TRUE, priority, subpriority);
    utils_uart4_puts("uart ready\r\n");
}

void init_timer() {
    // Timer Values
    const unsigned int tm_period_ms = 500;
    // PR2 = 500 (ms) / ((1/20000) (ms) * 256 (scaler)) 
    // = 39062.5 < (2^16-1) (65535), serve un timer a 16 bit
    const unsigned int tm_priority = 1;
    const unsigned int tm_subpriority = 0;

    utils_timer2_init (
            tm_period_ms, periph_bus_clock_hz, TMx_DIV_256, 
            TRUE, tm_priority, tm_subpriority);
    utils_uart4_puts("timer ready\r\n");
}


void init_leds() {
    utils_led_init();
    utils_uart4_puts("led ready\r\n");
}

void init_switch() {
    const unsigned int sw_priority = 1;
    const unsigned int sw_subpriority = 2;

    utils_switch_init_sw0_interrupt_cn(sw_priority, sw_subpriority);       
    utils_uart4_puts("switch ready\r\n");
}

void init_button() {
    const unsigned int priority = 1;
    const unsigned int subpriority = 3;

    utils_button_init_btn_c(TRUE, priority, subpriority);
    
    utils_uart4_puts("button ready\r\n");
}

typedef enum { 
    action_get_op, 
    action_led_on, 
    action_led_off 
} action_t;

action_t _current_action = action_get_op;

int _timer_elapsed = FALSE;
int _next_led = 0;
char _command[30];
int  _new_char_pos = 0;
int  _new_word = FALSE;

void get_operation() {
    //utils_uart4_gets(_command, 30);
    
    if(_new_word) {
        if(strcmp(_command, CMD_LED_ON) == 0) {
            utils_uart4_puts("leds turning on\r\n");
            _current_action = action_led_on;
        } else if(strcmp(_command, CMD_LED_OFF) == 0) {
            utils_uart4_puts("leds turning off\r\n");
            _current_action = action_led_off;
        } else {
            utils_uart4_puts("command unknown\r\n");
        }
        _new_word = FALSE;
        _new_char_pos = 0;
    }
}

void turn_led_on() {
    if(_timer_elapsed) {
        _timer_elapsed = FALSE;

        if(_next_led < 8) {
            utils_led_toggle(_next_led);
            _next_led++; 
        } else {
            utils_uart4_puts("all leds on\r\n");
            _current_action = action_get_op;
        }
    }
}

void turn_led_off() {
     if(_timer_elapsed) {
         _timer_elapsed = FALSE;

        if(_next_led >= 0) {
            utils_led_toggle(_next_led);
            _next_led--; 
        } else {
            utils_uart4_puts("all leds off\r\n");
            _current_action = action_get_op;
        }
    }
}


void main() {
    utils_common_macro_enable_interrupts();
    
    init_uart4();
    utils_uart4_puts("******** serie_4_ch5 ********\r\n");
    
    init_leds();
    init_timer();
    init_switch();
    init_button();
    
    utils_uart4_puts("******** commands: ledon|ledoff ********\r\n");
    memset(_command, 0, 30);

    while(1) {
        switch(_current_action) {
            case action_get_op: 
                get_operation();
                break;
            case action_led_on:
                turn_led_on();
                break;
            case action_led_off:
                turn_led_off();
                break;
        }
    }
}

void __attribute__(( interrupt(ipl1auto), vector(_TIMER_2_VECTOR)))
Timer2Handler(void) {
    _timer_elapsed = TRUE;
    IFS0bits.T2IF = 0;      // clear interrupt flag
}

void __attribute__(( interrupt(ipl6auto), vector(_UART_4_VECTOR)))
Uart4Handler(void) {
    char ch = U4RXREG;
    if(ch == '\r' || ch == '\n') {
        _new_word = TRUE;
        ch = '\0';
    }
    _command[_new_char_pos] = ch;
    _new_char_pos++;
    
	IFS2bits.U4RXIF = 0;    // clear interrupt flag
}

void __attribute__((interrupt(ipl1auto), vector(_CHANGE_NOTICE_VECTOR)))
SW0ChangeNotificationHandler(void){
    int value = PORTFbits.RF3;
    strcpy(_command, value ? CMD_LED_ON : CMD_LED_OFF);
    _new_word = TRUE;

    IFS1bits.CNFIF = 0;
}

void __attribute__((interrupt(ipl1auto), vector(_EXTERNAL_2_VECTOR)))
ButtonCHandler(void){
    int value = PORTFbits.RF0;
    strcpy(_command, value ? CMD_LED_ON : CMD_LED_OFF);
    _new_word = TRUE;

    IFS0bits.INT2IF = 0;       // clear interrupt flag
}

