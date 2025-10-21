#ifndef AUDIO_H
#define AUDIO_H
#include "utils/logger.h"
#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif
    void audio_init();
    void audio_destroy();

    void preloadEffect(jmethodID, va_list);
    void unloadEffect(jmethodID, va_list);
    jint playEffect(jmethodID, va_list);
    void setEffectVolume(jmethodID, va_list);
    void setEffectRate(jmethodID, va_list);
    void stopEffect(jmethodID, va_list);
    void pauseEffect(jmethodID, va_list);
    void resumeEffect(jmethodID, va_list);
    void pauseAllEffects(jmethodID, va_list);
    void resumeAllEffects(jmethodID, va_list);
    void stopAllEffects(jmethodID, va_list);
    jfloat getEffectsVolume(jmethodID, va_list);
    void setEffectsVolume(jmethodID, va_list);

    void preloadBackgroundMusic(jmethodID, va_list);
    void playBackgroundMusic(jmethodID, va_list);
    void stopBackgroundMusic(jmethodID, va_list);
    void pauseBackgroundMusic(jmethodID, va_list);
    void resumeBackgroundMusic(jmethodID, va_list);
    void rewindBackgroundMusic(jmethodID, va_list);
    jboolean isBackgroundMusicPlaying(jmethodID, va_list);
    void endBackgroundMusic(jmethodID, va_list);
    jfloat getBackgroundVolume(jmethodID, va_list);
    void setBackgroundVolume(jmethodID, va_list);

#ifdef __cplusplus
}
#endif

#endif