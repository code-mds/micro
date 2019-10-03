/* 
 * File:   Uart.h
 * Author: massi
 *
 * Created on 30. settembre 2019, 19:42
 */

void UART_ConfigurePins(void);
void UART_ConfigureUart(int baud);
int putU4(int c);
char getU4(void);
void putU4string(char szData[]);


