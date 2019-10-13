/* 
 * File:   Uart.h
 * Author: LSIB
 *
 * Created on 13. ottobre 2019, 09:00 */

 
void UART_ConfigurePins(void);
void UART_ConfigureUart(int baud);
int putU4( int c);
char getU4( void);
void putU4_string(char szData[]);
unsigned char getU4_string(void);

