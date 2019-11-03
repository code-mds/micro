/* 
 * File:   utils_timer.h
 * Author: massi
 *
 * Created on 13. ottobre 2019, 18:04
 */

#ifndef UTILS_TIMER_H
#define	UTILS_TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif
    typedef enum tm_use_interrupt_e { 
        TM_INTERRUPT_OFF = 0,
        TM_INTERRUPT_ON = 1
    } tm_use_interrupt_t;
    
    typedef enum tm1_prescaler_e { 
        TM1_DIV_1 = 0b00, 
        TM1_DIV_8 = 0b01, 
        TM1_DIV_64 = 0b10, 
        TM1_DIV_256 = 0b11 
    } tm1_prescaler_t;
    
    typedef enum tmx_prescaler_e { 
        TMx_DIV_1 = 0b000, 
        TMx_DIV_2 = 0b001, 
        TMx_DIV_4 = 0b010, 
        TMx_DIV_8 = 0b011, 
        TMx_DIV_16 = 0b100, 
        TMx_DIV_32 = 0b101, 
        TMx_DIV_64 = 0b110, 
        TMx_DIV_256 = 0b111 
    } tmx_prescaler_t;
    
    // prescaler 0..3: 1,8,64,256
    void utils_timer1_init(
            int period_ms, int bus_freq, tm1_prescaler_t prescaler, 
            tm_use_interrupt_t use_interrupt, int priority, int sub_priority);
    
    // prescaler 0..7: 1,2,4,8,16,32,64,256
    void utils_timer2_init(
            int period_ms, int bus_freq, tmx_prescaler_t prescaler, 
            tm_use_interrupt_t use_interrupt, int priority, int sub_priority);

    void utils_timer23_init_32bit(
            int period_ms, int bus_freq, tmx_prescaler_t prescaler, 
            tm_use_interrupt_t use_interrupt, int priority, int sub_priority);
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_TIMER_H */

