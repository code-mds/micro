/* 
 * File:   utils_led.h
 * Author: massi
 *
 * Created on 5. ottobre 2019, 16:03
 */

#ifndef UTILS_LED_H
#define	UTILS_LED_H

#ifdef	__cplusplus
extern "C" {
#endif

    void utils_led_init();
    void utils_led_toggle(int idx);
    void utils_led_set(int idx, int value);
    int utils_led_get(int idx);

    enum {
        LED_OFF = 0,
        LED_ON = 1
    };
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_LED_H */

