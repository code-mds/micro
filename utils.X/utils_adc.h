/* 
 * File:   utils_adc.h
 * Author: massi
 *
 * Created on 25. novembre 2019, 20:38
 */

#ifndef UTILS_ADC_H
#define	UTILS_ADC_H

#include "utils_timer.h"

#ifdef	__cplusplus
extern "C" {
#endif

    void utils_adc_init();
    int utils_adc_get_int(void(*delay)(int), int delay_ms);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_ADC_H */

