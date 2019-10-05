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
    void utils_led_set(int idx, int value);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_LED_H */

