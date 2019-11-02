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

    // prescaler 0..3: 1,8,64,256
    void utils_timer1_init(int period_ms, int bus_freq, int prescaler_idx, 
            int use_interrupt, int priority, int sub_priority);
    
    // prescaler 0..7: 1,2,4,8,16,32,64,256
    void utils_timer2_init(int period_ms, int bus_freq, int prescaler_idx, 
            int use_interrupt, int priority, int sub_priority);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_TIMER_H */

