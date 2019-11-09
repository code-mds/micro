/* 
 * File:   Uart.h
 * Author: massi
 *
 * Created on 30. settembre 2019, 19:42
 */

#ifndef UTILS_UART_H
#define	UTILS_UART_H

#include "utils_common.h"

#ifdef	__cplusplus
extern "C" {
#endif
    void utils_uart4_init(int baud, int pbus_clock);
    void utils_uart4_init_interrupt(int baud, int pbus_clock, 
        use_interrupt_t use_interrupt, int priority, int sub_priority);
    
    int utils_uart4_putc(int c);
    char utils_uart4_getc(void);
    void utils_uart4_puts(const char* buffer);
    int utils_uart4_gets(char* buffer, int max_sz);
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_UART_H */



