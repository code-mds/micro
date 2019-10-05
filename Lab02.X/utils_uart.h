/* 
 * File:   Uart.h
 * Author: massi
 *
 * Created on 30. settembre 2019, 19:42
 */

#ifndef UTILS_UART_H
#define	UTILS_UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    void utils_uart_ConfigurePins(void);
    void utils_uart_ConfigureUart(int baud);
    int utils_uart_putU4(int c);
    char utils_uart_getU4(void);
    void utils_uart_putU4string(char szData[]);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_UART_H */



