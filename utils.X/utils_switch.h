/* 
 * File:   utils_switch.h
 * Author: massi
 *
 * Created on 5. ottobre 2019, 15:50
 */

#ifndef UTILS_SWITCH_H
#define	UTILS_SWITCH_H

#include "utils_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

    void utils_switch_init();
    void utils_switch_init_sw0_interrupt_cn(int priority, int subpriority);
    int utils_switch_get(int idx);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_SWITCH_H */

