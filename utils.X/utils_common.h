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
        FALSE = 0,
        TRUE = 1
    } boolean_t;
    
    typedef enum {
        OUTPUT = 0,
        INPUT = 1
    } io_t;

    typedef enum {
        DIGITAL = 0,
        ANALOG = 1
    } digital_t;
  
    typedef enum {
        INT_PRIORITY_DISABLED = 0,
        INT_PRIORITY_1 = 1,
        INT_PRIORITY_2 = 2,
        INT_PRIORITY_3 = 3,
        INT_PRIORITY_4 = 4,
        INT_PRIORITY_5 = 5,
        INT_PRIORITY_6 = 6,
        INT_PRIORITY_7 = 7
    } int_priority_t;

    typedef enum {
        INT_SUB_PRIORITY_DISABLED = 0,
        INT_SUB_PRIORITY_0 = 0,
        INT_SUB_PRIORITY_1 = 1,
        INT_SUB_PRIORITY_2 = 2,
        INT_SUB_PRIORITY_3 = 3
    } int_subpriority_t;

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

