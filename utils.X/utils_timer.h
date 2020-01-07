/* 
 * File:   utils_timer.h
 * Author: massi
 *
 * Created on 13. ottobre 2019, 18:04
 */

#ifndef UTILS_TIMER_H
#define	UTILS_TIMER_H

#include "utils_common.h"

#ifdef	__cplusplus
extern "C" {
#endif    
    typedef enum { 
        TM1_DIV_1 = 0b00, 
        TM1_DIV_8 = 0b01, 
        TM1_DIV_64 = 0b10, 
        TM1_DIV_256 = 0b11 
    } tm1_prescaler_t;
    
    typedef enum { 
        TMx_DIV_1 = 0b000, 
        TMx_DIV_2 = 0b001, 
        TMx_DIV_4 = 0b010, 
        TMx_DIV_8 = 0b011, 
        TMx_DIV_16 = 0b100, 
        TMx_DIV_32 = 0b101, 
        TMx_DIV_64 = 0b110, 
        TMx_DIV_256 = 0b111 
    } tmx_prescaler_t;
    
    int utils_timer_calc_pr_16bit(int period_ms, int bus_freq, int prescaler_val);
    // prescaler 0..3: 1,8,64,256
    void utils_timer1_init(
            int period_ms, int bus_freq, tm1_prescaler_t prescaler, 
            boolean_t use_interrupt, int priority, int sub_priority);
    void utils_timer1_stop();

    void utils_timer1_delay(int period_ms, int bus_freq, tm1_prescaler_t prescaler);
    void utils_timer2_delay(int period_ms, int bus_freq, tmx_prescaler_t prescaler);

    // prescaler 0..7: 1,2,4,8,16,32,64,256
    void utils_timer2_init(
            int period_ms, int bus_freq, tmx_prescaler_t prescaler, 
            boolean_t use_interrupt, int priority, int sub_priority);

    void utils_timer23_init_32bit(
            int period_ms, int bus_freq, tmx_prescaler_t prescaler, 
            boolean_t use_interrupt, int priority, int sub_priority);
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_TIMER_H */

