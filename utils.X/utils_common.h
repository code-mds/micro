/* 
 * File:   utils_common.h
 * Author: massi
 *
 * Created on 14. ottobre 2019, 18:43
 */

#ifndef UTILS_COMMON_H
#define	UTILS_COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif
    void utils_common_delay(int counter);
    void utils_common_tolower(char* str);
    
    typedef enum { 
        INTERRUPT_OFF = 0,
        INTERRUPT_ON = 1
    } use_interrupt_t;
    
// Macro Per Interrupt   
#define utils_common_macro_enable_interrupts()\
{   unsigned int val = 0;\
    asm volatile("mfc0 %0,$13":"=r"(val));\
    val |= 0x00800000;\
    asm volatile("mfc0 %0,$13":"+r"(val));\
    INTCONbits.MVEC = 1;\
    __builtin_enable_interrupts();}
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_COMMON_H */

