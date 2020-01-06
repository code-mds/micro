/* 
 * File:   utils_lcd.h
 * Author: massi
 *
 * Created on 24. novembre 2019, 15:29
 */

#ifndef UTILS_LCD_H
#define	UTILS_LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define LCDDATA 1 // RS = 1 ; access data register
    #define LCDCMD 0 // RS = 0 ; access command register
    #define PMDATA PMDIN // PMP data buffer
    
    void utils_lcd_init(void (*delay_fn)(int));
    char utils_lcd_read(int addr);
    void utils_lcd_write(int addr, char c);
    void utils_lcd_write_str(const char *str);
    void utils_lcd_write_int(int value);

    // check if busy
    #define utils_lcd_busy() utils_lcd_read(LCDCMD) & 0x80
    // send ASCII data to LCD
    #define utils_lcd_put(d) utils_lcd_write(LCDDATA, (d) )
    // send generic command to LCD
    #define utils_lcd_cmd(c) utils_lcd_write(LCDCMD, (c) )
    // move cursor to the first char of first row (home)
    #define utils_lcd_home() utils_lcd_write(LCDCMD, 2)
    // clear content
    #define utils_lcd_clr()  utils_lcd_write(LCDCMD, 1)

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_LCD_H */

