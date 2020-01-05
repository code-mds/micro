/* 
 * File:   utils_button.h
 * Author: massi
 *
 * Created on 9. novembre 2019, 16:48
 */

#ifndef UTILS_BUTTON_H
#define	UTILS_BUTTON_H

#include "utils_common.h"

#ifdef	__cplusplus
extern "C" {
#endif

    void utils_button_init_btn_c(boolean_t use_interrupt, int priority, 
            int sub_priority);
    int utils_button_get_btn_c();
    
    void utils_button_init_btn_u();
    int utils_button_get_btn_u();
    
    void utils_button_init_btn_r();
    int utils_button_get_btn_r();
    
    void utils_button_init_btn_l();
    int utils_button_get_btn_l();

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_BUTTON_H */

