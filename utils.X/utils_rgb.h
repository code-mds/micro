/* 
 * File:   utils_rgb.h
 * Author: massi
 *
 * Created on 5. gennaio 2020, 18:58
 */

#ifndef UTILS_RGB_H
#define	UTILS_RGB_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        verde,
        giallo,
        rosso,
        blu,
        off
    } color_t;


    void utils_rgb_init();
    void utils_rgb_init_int();

    void utils_rgb_set_color(color_t color);
    void utils_rgb_set_int(int r, int g, int b);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_RGB_H */

