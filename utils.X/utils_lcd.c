#include <p32xxxx.h>
#include "utils_lcd.h"
#include "utils_timer.h"

#define tris_LCD_DISP_RS    TRISBbits.TRISB15
#define lat_LCD_DISP_RS     LATBbits.LATB15
#define ansel_LCD_DISP_RS   ANSELBbits.ANSB15
#define rp_LCD_DISP_RS      RPB15R

#define tris_LCD_DISP_RW    TRISDbits.TRISD5
#define lat_LCD_DISP_RW     LATDbits.LATD5
#define rp_LCD_DISP_RW      RPD5R

#define tris_LCD_DISP_EN    TRISDbits.TRISD4
#define lat_LCD_DISP_EN     LATDbits.LATD4
#define rp_LCD_DISP_EN      RPD4R

#define tris_LCD_DATA       TRISE
#define lat_LCD_DATA        LATE
#define prt_LCD_DATA        PORTE
#define msk_LCD_DATA        0xFF
#define lat_LCD_DATA_ADDR   0xBF886440

#define ansel_LCD_DB2       ANSELEbits.ANSE2
#define ansel_LCD_DB4       ANSELEbits.ANSE4
#define ansel_LCD_DB5       ANSELEbits.ANSE5
#define ansel_LCD_DB6       ANSELEbits.ANSE6
#define ansel_LCD_DB7       ANSELEbits.ANSE7

void utils_lcd_config_pins()
{
    // set control pins as digital outputs.
    tris_LCD_DISP_RS = 0;
    tris_LCD_DISP_RW = 0;
    tris_LCD_DISP_EN = 0;
    
    // disable analog (set pins as digital))
    ansel_LCD_DISP_RS = 0;
    
    // default (IO) function for remapable pins
    rp_LCD_DISP_RS = 0;
    rp_LCD_DISP_RW = 0;
    rp_LCD_DISP_EN = 0;
    
    // make data pins digital (disable analog)
    ansel_LCD_DB2 = 0;
    ansel_LCD_DB4 = 0;
    ansel_LCD_DB5 = 0;
    ansel_LCD_DB6 = 0;
    ansel_LCD_DB7 = 0;
}

void utils_lcd_init(int bus_freq, int prescaler) {
    utils_lcd_config_pins();
    
    // default (IO) function for remapable pins
    RPB15R = 0;  // rp_LCD_DISP_RS = 0;
    RPD5R = 0;   // rp_LCD_DISP_RW = 0;
    RPD4R = 0;   // rp_LCD_DISP_EN = 0;
    
    // make data pins digital (disable analog)
    ansel_LCD_DB2 = 0;
    ansel_LCD_DB4 = 0;
    ansel_LCD_DB5 = 0;
    ansel_LCD_DB6 = 0;
    ansel_LCD_DB7 = 0;
    
    // PMP initialization
    PMCON = 0x83BF ; // Enable the PMP, long waits
    PMMODE = 0x3FF ; // Master Mode 1
    PMAEN = 0x0001 ; // PMA0 enabled   

    utils_timer1_delay(30, bus_freq, prescaler);
    
    PMADDR = LCDCMD ; // command register ( ADDR = 0)
    PMDATA = 0x38 ; // 8-bit interface, 2 lines, 5x7
    utils_timer1_delay(1, bus_freq, prescaler) ; // >48 us
    
    PMDATA = 0x0c ; // ON, no cursor, no blink
    utils_timer1_delay(1, bus_freq, prescaler) ; // >48 us
    
    PMDATA = 0x01 ; // clear display
    utils_timer1_delay(2, bus_freq, prescaler) ; // >1.6 ms

    PMDATA = 0x06 ; // increment cursor, no shift
    utils_timer1_delay(2, bus_freq, prescaler) ; // >1.6 ms
}

char utils_lcd_read(int addr) {
    while(PMMODEbits.BUSY) {} // wait for PMP available
    
    PMADDR = addr; // select the command address
    int dummy = PMDATA; // init read cycle , dummy read
    
    while(PMMODEbits.BUSY) {} // wait for PMP to be available
    return (PMDATA); // read the status register
}

void utils_lcd_write(int addr, char c) {
    while (utils_lcd_busy()) {} // check busy flag of LCD
    while (PMMODEbits.BUSY) {} // wait for PMP available
    PMADDR = addr;
    PMDATA = c;
}

void utils_lcd_write_str(const char *str) {
    while (*str) {
        utils_lcd_put(*str++);
    }
}
