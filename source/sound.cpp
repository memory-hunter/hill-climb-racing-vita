#include "sound.h"

#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>

#include "soloud.h"
#include "soloud_wav.h"

struct SoundManager
{
    SoLoud::Soloud soloud;
    std::map<std::string, SoLoud::Wav *> sounds;
    std::map<std::string, std::vector<SoLoud::handle>> handles;
    float volumeLeft = 0.5f;
    float volumeRight = 0.5f;
    std::mutex mutex;
} gSoundManager;

void preloadEffect(jmethodID, va_list args)
{
    jstring jpath = va_arg(args, jstring);
    const char *relPath = jni.GetStringUTFChars(jpath, nullptr);
    std::string relative(relPath ? relPath : "");
    jni.ReleaseStringUTFChars(jpath, relPath);

    std::string fullPath = "ux0:/data/hcr/assets/" + relative;

    std::lock_guard<std::mutex> lock(gSoundManager.mutex);

    if (gSoundManager.sounds.find(fullPath) != gSoundManager.sounds.end())
    {
        l_debug("preloadEffect: already loaded: %s", fullPath.c_str());
        return;
    }

    auto *wav = new SoLoud::Wav();
    int res = wav->load(fullPath.c_str());
    if (res != SoLoud::SO_NO_ERROR)
    {
        l_debug("preloadEffect: failed to load %s (SoLoud error %d)", fullPath.c_str(), res);
        delete wav;
        return;
    }

    gSoundManager.sounds[fullPath] = wav;
    l_debug("preloadEffect: loaded %s", fullPath.c_str());
}

void unloadEffect(jmethodID, va_list args)
{
    jstring jpath = va_arg(args, jstring);
    const char *path = jni.GetStringUTFChars(jpath, nullptr);
    std::string key(path);
    jni.ReleaseStringUTFChars(jpath, path);

    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    auto it = gSoundManager.sounds.find(key);
    if (it != gSoundManager.sounds.end())
    {

        auto &handles = gSoundManager.handles[key];
        for (auto h : handles)
        {
            gSoundManager.soloud.stop(h);
        }
        handles.clear();
        delete it->second;
        gSoundManager.sounds.erase(it);
    }
}

jint playEffect(jmethodID, va_list args)
{
    jstring jpath = va_arg(args, jstring);
    jboolean isLoop = va_arg(args, jint);
    jfloat volume = va_arg(args, jdouble);
    jfloat rate = va_arg(args, jdouble);

    const char *relPath = jni.GetStringUTFChars(jpath, nullptr);
    std::string relative(relPath ? relPath : "");
    jni.ReleaseStringUTFChars(jpath, relPath);

    std::string fullPath = "ux0:/data/hcr/assets/" + relative;
    l_debug("playEffect: called for %s (loop=%d, vol=%.2f, rate=%.2f)", fullPath.c_str(), isLoop, volume, rate);

    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    auto it = gSoundManager.sounds.find(fullPath);
    if (it == gSoundManager.sounds.end())
    {
        l_debug("playEffect: sound not preloaded, attempting to load %s", fullPath.c_str());

        auto *wav = new SoLoud::Wav();
        int res = wav->load(fullPath.c_str());
        if (res != SoLoud::SO_NO_ERROR)
        {
            l_debug("playEffect: failed to load %s (SoLoud error %d)", fullPath.c_str(), res);
            delete wav;
            return -1;
        }
        gSoundManager.sounds[fullPath] = wav;
        it = gSoundManager.sounds.find(fullPath);
        l_debug("playEffect: loaded on-demand %s", fullPath.c_str());
    }

    volume = std::fmax(0.0f, std::fmin(volume, 1.0f));
    rate = std::fmax(0.5f, std::fmin(rate, 2.0f));

    auto *wav = it->second;
    SoLoud::handle handle = gSoundManager.soloud.play(*wav, volume, 0.0f, isLoop);
    if (handle < 0)
    {
        l_debug("playEffect: failed to play %s", fullPath.c_str());
        return -1;
    }

    gSoundManager.soloud.setRelativePlaySpeed(handle, rate);
    gSoundManager.handles[fullPath].push_back(handle);

    l_debug("playEffect: playing %s (handle=%d)", fullPath.c_str(), handle);
    return handle;
}

void setEffectVolume(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    jdouble volume = va_arg(args, jdouble);
    volume = std::fmax(0.0f, std::fmin(volume, 1.0f));
    gSoundManager.soloud.setVolume(streamID, volume);
}

void setEffectRate(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    jdouble rate = va_arg(args, jdouble);
    rate = std::fmax(0.5f, std::fmin(rate, 2.0f));
    gSoundManager.soloud.setRelativePlaySpeed(streamID, rate);
}

void stopEffect(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    gSoundManager.soloud.stop(streamID);

    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    for (auto &pair : gSoundManager.handles)
    {
        auto &vec = pair.second;
        vec.erase(std::remove(vec.begin(), vec.end(), streamID), vec.end());
    }
}

void pauseEffect(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    gSoundManager.soloud.setPause(streamID, true);
}

void resumeEffect(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    gSoundManager.soloud.setPause(streamID, false);
}

void pauseAllEffects(jmethodID, va_list)
{
    gSoundManager.soloud.setPauseAll(true);
}

void resumeAllEffects(jmethodID, va_list)
{
    gSoundManager.soloud.setPauseAll(false);
}

void stopAllEffects(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    for (auto &pair : gSoundManager.handles)
    {
        for (auto h : pair.second)
        {
            gSoundManager.soloud.stop(h);
        }
        pair.second.clear();
    }
}

jfloat getEffectsVolume(jmethodID, va_list)
{
    return (gSoundManager.volumeLeft + gSoundManager.volumeRight) * 0.5f;
}

void setEffectsVolume(jmethodID, va_list args)
{
    jdouble volume = va_arg(args, jdouble);
    volume = std::fmax(0.0f, std::fmin(volume, 1.0f));

    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    gSoundManager.volumeLeft = gSoundManager.volumeRight = volume;
    for (auto &pair : gSoundManager.handles)
    {
        for (auto h : pair.second)
        {
            gSoundManager.soloud.setVolume(h, volume);
        }
    }
}

void sound_destroy()
{
    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    for (auto &pair : gSoundManager.handles)
    {
        for (auto h : pair.second)
            gSoundManager.soloud.stop(h);
    }
    gSoundManager.handles.clear();

    for (auto &pair : gSoundManager.sounds)
        delete pair.second;
    gSoundManager.sounds.clear();

    gSoundManager.soloud.deinit();
}

void sound_init()
{
    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    SoLoud::result res = gSoundManager.soloud.init();
    if (res != SoLoud::SO_NO_ERROR)
    {
        l_error("[Sound] Failed to initialize SoLoud: %s",
                gSoundManager.soloud.getErrorString(res));
        return;
    }

    gSoundManager.sounds.clear();
    gSoundManager.handles.clear();

    gSoundManager.volumeLeft = 0.5f;
    gSoundManager.volumeRight = 0.5f;

    l_debug("[Sound] SoLoud initialized successfully.");
}