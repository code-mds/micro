/* 
 * File:   utils_audio.h
 * Author: massi
 *
 * Created on 5. gennaio 2020, 18:30
 */

#ifndef UTILS_AUDIO_H
#define	UTILS_AUDIO_H

#ifdef	__cplusplus
extern "C" {
#endif

    void utils_audio_init(int pbus_clk);
    void utils_audio_beep_start();
    void utils_audio_beep_stop();
    
#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_AUDIO_H */

