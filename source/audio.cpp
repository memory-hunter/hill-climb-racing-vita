#include "audio.h"
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"
#include <mutex>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

struct AudioSystem
{
    SoLoud::Soloud soloud;
    std::mutex mutex;
} gAudioSystem;

struct MusicManager
{
    SoLoud::WavStream bgm;
    bool hasBgmLoaded = false;
    float volumeLeft = 0.5f;
    float volumeRight = 0.5f;
    SoLoud::handle handle = 0;
    std::string currentPath;
    std::mutex mutex;
} gMusicManager;

struct SoundManager
{
    std::map<std::string, SoLoud::Wav *> sounds;
    std::map<std::string, std::vector<SoLoud::handle>> handles;
    float volumeLeft = 0.5f;
    float volumeRight = 0.5f;
    std::mutex mutex;
} gSoundManager;

void audio_init()
{
    std::lock_guard<std::mutex> lock(gAudioSystem.mutex);

    #ifdef AUDIO_DEBUG
    l_debug("[Audio] Initializing SoLoud...");
    #endif
    SoLoud::result res = gAudioSystem.soloud.init(
        SoLoud::Soloud::CLIP_ROUNDOFF,
        SoLoud::Soloud::VITA_HOMEBREW,
        44100,
        2048,
        2);
    if (res != SoLoud::SO_NO_ERROR)
    {
        l_error("[Audio] Failed to initialize SoLoud: %s",
                gAudioSystem.soloud.getErrorString(res));
        return;
    }

    gAudioSystem.soloud.setGlobalVolume(1.0f);
    gAudioSystem.soloud.setMaxActiveVoiceCount(64);
    gAudioSystem.soloud.setVisualizationEnable(false);

    gSoundManager.sounds.clear();
    gSoundManager.handles.clear();
    gSoundManager.volumeLeft = 0.5f;
    gSoundManager.volumeRight = 0.5f;

    gMusicManager.hasBgmLoaded = false;
    gMusicManager.volumeLeft = 0.5f;
    gMusicManager.volumeRight = 0.5f;
    gMusicManager.currentPath.clear();
    gMusicManager.handle = 0;

    #ifdef AUDIO_DEBUG
    l_debug("[Audio] SoLoud initialized successfully.");
    #endif
}

void audio_destroy()
{
    std::lock_guard<std::mutex> lock(gAudioSystem.mutex);

    for (auto &pair : gSoundManager.handles)
    {
        for (auto h : pair.second)
            gAudioSystem.soloud.stop(h);
    }
    gSoundManager.handles.clear();

    for (auto &pair : gSoundManager.sounds)
        delete pair.second;
    gSoundManager.sounds.clear();

    gAudioSystem.soloud.stopAll();
    gMusicManager.bgm.stop();
    gMusicManager.hasBgmLoaded = false;
    gMusicManager.currentPath.clear();

    gAudioSystem.soloud.deinit();

    #ifdef AUDIO_DEBUG
    l_debug("[Audio] SoLoud destroyed.");
    #endif
}

// sound
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
        #ifdef AUDIO_DEBUG
        l_debug("preloadEffect: already loaded: %s", fullPath.c_str());
        #endif
        return;
    }

    auto *wav = new SoLoud::Wav();
    int res = wav->load(fullPath.c_str());
    if (res != SoLoud::SO_NO_ERROR)
    {
        #ifdef AUDIO_DEBUG
        l_debug("preloadEffect: failed to load %s (SoLoud error %d)", fullPath.c_str(), res);
        #endif
        delete wav;
        return;
    }

    gSoundManager.sounds[fullPath] = wav;
    #ifdef AUDIO_DEBUG
    l_debug("preloadEffect: loaded %s", fullPath.c_str());
    #endif
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
            gAudioSystem.soloud.stop(h);
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
    #ifdef AUDIO_DEBUG
    l_debug("playEffect: called for %s (loop=%d, vol=%.2f, rate=%.2f)", fullPath.c_str(), isLoop, volume, rate);
    #endif

    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    auto it = gSoundManager.sounds.find(fullPath);
    if (it == gSoundManager.sounds.end())
    {
        #ifdef AUDIO_DEBUG
        l_debug("playEffect: sound not preloaded, attempting to load %s", fullPath.c_str());
        #endif

        auto *wav = new SoLoud::Wav();
        int res = wav->load(fullPath.c_str());
        if (res != SoLoud::SO_NO_ERROR)
        {
            #ifdef AUDIO_DEBUG
            l_debug("playEffect: failed to load %s (SoLoud error %d)", fullPath.c_str(), res);
            #endif
            delete wav;
            return -1;
        }
        wav->setLooping(isLoop);
        gSoundManager.sounds[fullPath] = wav;
        it = gSoundManager.sounds.find(fullPath);
        #ifdef AUDIO_DEBUG
        l_debug("playEffect: loaded on-demand %s", fullPath.c_str());
        #endif
    }

    volume = std::fmax(0.0f, std::fmin(volume, 1.0f));
    rate = std::fmax(0.5f, std::fmin(rate, 2.0f));

    auto *wav = it->second;
    wav->setLooping(isLoop);
    SoLoud::handle handle = gAudioSystem.soloud.play(*wav, volume, 0, true, 0);
    if (handle < 0)
    {
        #ifdef AUDIO_DEBUG
        l_debug("playEffect: failed to play %s", fullPath.c_str());
        #endif
        return -1;
    }

    if (isLoop)
        gSoundManager.handles[fullPath].push_back(handle);

    gAudioSystem.soloud.setPause(handle, false);

    #ifdef AUDIO_DEBUG
    l_debug("playEffect: playing %s (handle=%d)", fullPath.c_str(), handle);
    #endif

    return handle;
}

void setEffectVolume(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    jdouble volume = va_arg(args, jdouble);
    #ifdef AUDIO_DEBUG
    l_debug("setEffectVolume called for stream %d to volume %.2f", streamID, volume);
    #endif
    volume = std::fmax(0.0f, std::fmin(volume, 1.0f));
    gAudioSystem.soloud.setVolume(streamID, volume);
}

void setEffectRate(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    jdouble rate = va_arg(args, jdouble);
    #ifdef AUDIO_DEBUG
    l_debug("setEffectRate called for stream %d to rate %.2f", streamID, rate);
    #endif
    rate = std::fmax(0.5f, std::fmin(rate, 2.0f));
    gAudioSystem.soloud.setRelativePlaySpeed(streamID, rate);
}

void stopEffect(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    #ifdef AUDIO_DEBUG
    l_debug("stopEffect called for stream %d", streamID);
    #endif
    gAudioSystem.soloud.stop(streamID);

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
    #ifdef AUDIO_DEBUG
    l_debug("pauseEffect called for stream %d", streamID);
    #endif
    gAudioSystem.soloud.setPause(streamID, true);
}

void resumeEffect(jmethodID, va_list args)
{
    jint streamID = va_arg(args, jint);
    #ifdef AUDIO_DEBUG
    l_debug("resumeEffect called for stream %d", streamID);
    #endif
    gAudioSystem.soloud.setPause(streamID, false);
}

void pauseAllEffects(jmethodID, va_list)
{
    gAudioSystem.soloud.setPauseAll(true);
}

void resumeAllEffects(jmethodID, va_list)
{
    gAudioSystem.soloud.setPauseAll(false);
}

void stopAllEffects(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    for (auto &pair : gSoundManager.handles)
    {
        for (auto h : pair.second)
        {
            gAudioSystem.soloud.stop(h);
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
    #ifdef AUDIO_DEBUG
    l_debug("setEffectsVolume called to volume %.2f", volume);
    #endif
    std::lock_guard<std::mutex> lock(gSoundManager.mutex);
    gSoundManager.volumeLeft = gSoundManager.volumeRight = volume;
    for (auto &pair : gSoundManager.handles)
    {
        for (auto h : pair.second)
        {
            gAudioSystem.soloud.setVolume(h, volume);
        }
    }
}

// music

void preloadBackgroundMusic(jmethodID, va_list args)
{
    jstring jpath = va_arg(args, jstring);
    const char *relPath = jni.GetStringUTFChars(jpath, nullptr);
    std::string relative(relPath ? relPath : "");
    jni.ReleaseStringUTFChars(jpath, relPath);

    std::string fullPath = "ux0:/data/hcr/assets/" + relative;

    std::lock_guard<std::mutex> lock(gMusicManager.mutex);

    if (gMusicManager.currentPath == fullPath && gMusicManager.hasBgmLoaded)
    {
        #ifdef AUDIO_DEBUG
        l_debug("preloadBackgroundMusic: already loaded %s", fullPath.c_str());
        #endif
        return;
    }

    gMusicManager.bgm.stop();
    gMusicManager.currentPath = fullPath;

    if (gMusicManager.bgm.load(fullPath.c_str()) != SoLoud::SO_NO_ERROR)
    {
        l_error("preloadBackgroundMusic: failed to load %s", fullPath.c_str());
        gMusicManager.hasBgmLoaded = false;
        return;
    }

    gMusicManager.hasBgmLoaded = true;
    #ifdef AUDIO_DEBUG
    l_debug("preloadBackgroundMusic: loaded %s", fullPath.c_str());
    #endif
}

void playBackgroundMusic(jmethodID, va_list args)
{
    jstring jpath = va_arg(args, jstring);
    jboolean isLoop = va_arg(args, jint);

    const char *relPath = jni.GetStringUTFChars(jpath, nullptr);
    std::string relative(relPath ? relPath : "");
    jni.ReleaseStringUTFChars(jpath, relPath);

    std::string fullPath = "ux0:/data/hcr/assets/" + relative;

    std::lock_guard<std::mutex> lock(gMusicManager.mutex);

    if (!gMusicManager.hasBgmLoaded || gMusicManager.currentPath != fullPath)
    {
        gMusicManager.bgm.stop();
        if (gMusicManager.bgm.load(fullPath.c_str()) != SoLoud::SO_NO_ERROR)
        {
            l_error("playBackgroundMusic: failed to load %s", fullPath.c_str());
            return;
        }
        gMusicManager.currentPath = fullPath;
        gMusicManager.hasBgmLoaded = true;
    }

    if (isLoop)
        gMusicManager.bgm.mFlags |= SoLoud::AudioSource::SHOULD_LOOP;
    else
        gMusicManager.bgm.mFlags &= ~SoLoud::AudioSource::SHOULD_LOOP;

    gMusicManager.handle = gAudioSystem.soloud.playBackground(gMusicManager.bgm);
    gAudioSystem.soloud.setVolume(gMusicManager.handle,
                                  (gMusicManager.volumeLeft + gMusicManager.volumeRight) / 2.0f);

    #ifdef AUDIO_DEBUG
    l_debug("playBackgroundMusic: playing %s (loop=%d)", fullPath.c_str(), isLoop);
    #endif
}

void stopBackgroundMusic(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    if (gMusicManager.hasBgmLoaded)
    {
        gAudioSystem.soloud.stopAll();
        #ifdef AUDIO_DEBUG
        l_debug("stopBackgroundMusic: stopped");
        #endif
    }
}

void pauseBackgroundMusic(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    if (gMusicManager.handle)
    {
        gAudioSystem.soloud.setPause(gMusicManager.handle, true);
        #ifdef AUDIO_DEBUG
        l_debug("pauseBackgroundMusic");
        #endif
    }
}

void resumeBackgroundMusic(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    if (gMusicManager.handle)
    {
        gAudioSystem.soloud.setPause(gMusicManager.handle, false);
        #ifdef AUDIO_DEBUG
        l_debug("resumeBackgroundMusic");
        #endif
    }
}

void rewindBackgroundMusic(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    if (gMusicManager.hasBgmLoaded)
    {
        gAudioSystem.soloud.stopAll();
        gMusicManager.handle = gAudioSystem.soloud.play(gMusicManager.bgm);
        #ifdef AUDIO_DEBUG
        l_debug("rewindBackgroundMusic");
        #endif
    }
}

jboolean isBackgroundMusicPlaying(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    if (!gMusicManager.hasBgmLoaded)
        return JNI_FALSE;

    bool playing = gAudioSystem.soloud.isValidVoiceHandle(gMusicManager.handle) &&
                   gAudioSystem.soloud.getPause(gMusicManager.handle) == 0;
    return playing ? JNI_TRUE : JNI_FALSE;
}

void endBackgroundMusic(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    gAudioSystem.soloud.stopAll();
    gMusicManager.bgm.stop();
    gMusicManager.hasBgmLoaded = false;
    gMusicManager.currentPath.clear();
    gAudioSystem.soloud.deinit();
    #ifdef AUDIO_DEBUG
    l_debug("endBackgroundMusic: deinitialized SoLoud");
    #endif
}

jfloat getBackgroundVolume(jmethodID, va_list)
{
    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    return (gMusicManager.volumeLeft + gMusicManager.volumeRight) * 0.5f;
}

void setBackgroundVolume(jmethodID, va_list args)
{
    jfloat volume = va_arg(args, jdouble);
    volume = std::fmax(0.0f, std::fmin(1.0f, volume));

    std::lock_guard<std::mutex> lock(gMusicManager.mutex);
    gMusicManager.volumeLeft = gMusicManager.volumeRight = volume;
    if (gMusicManager.hasBgmLoaded && gMusicManager.handle)
    {
        gAudioSystem.soloud.setVolume(gMusicManager.handle, volume);
    }
    #ifdef AUDIO_DEBUG
    l_debug("setBackgroundVolume: %.2f", volume);
    #endif
}
