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
    void utils_timer1_init(int ms, int freq, int prescaler);

    // prescaler 0..7: 1,2,4,8,16,32,64,256
    void utils_timer2_init(int ms, int freq, int prescaler);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_TIMER_H */

