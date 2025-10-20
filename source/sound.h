#ifndef SOUND_H
#define SOUND_H

#include "soloud.h"
#include "soloud_wav.h"
#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>
#include <map>
#include <vector>
#include <string>
#include <mutex>

struct SoundManager
{
    SoLoud::Soloud soloud;
    std::map<std::string, SoLoud::Wav *> sounds;
    std::map<std::string, std::vector<SoLoud::handle>> handles;
    float volumeLeft = 0.5f;
    float volumeRight = 0.5f;
    std::mutex mutex;
};

extern SoundManager gSoundManager;

jint preloadEffect(jmethodID, va_list);
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
void endSoundSystem(jmethodID, va_list);

#endif
