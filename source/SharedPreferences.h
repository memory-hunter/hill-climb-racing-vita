#ifndef SOLOADER_SHAREDPREFERENCES_H
#define SOLOADER_SHAREDPREFERENCES_H

#include <falso_jni/jni.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void prefs_init();

    jboolean getBoolForKey(jmethodID id, va_list args);

    jint getIntegerForKey(jmethodID id, va_list args);

    void setIntegerForKey(jmethodID id, va_list args);

    jobject getStringForKey(jmethodID id, va_list args);

    void setBoolForKey(jmethodID id, va_list args);

    void setStringForKey(jmethodID id, va_list args);

    void prefs_destroy();
    void startPrefsSaver(int intervalSec);
    static void prefsSaverLoop();

#ifdef __cplusplus
};
#endif

#endif // SOLOADER_SHAREDPREFERENCES_H
