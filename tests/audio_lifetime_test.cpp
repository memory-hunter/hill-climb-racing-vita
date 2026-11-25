#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include "../source/audio.cpp"

static JNINativeInterface table = {};
JNIEnv jni = {&table};
extern "C" void _log_print(int, const char *, ...) {}

static void invoke(void (*method)(jmethodID, va_list), ...) {
    va_list args;
    va_start(args, method);
    method(nullptr, args);
    va_end(args);
}

int main() {
    table.GetStringUTFChars = [](JNIEnv *, jstring value, jboolean *) {
        return value ? static_cast<const char *>(strdup(reinterpret_cast<const char *>(value))) : nullptr;
    };
    table.ReleaseStringUTFChars = [](JNIEnv *, jstring, char *value) {
        free(value);
    };
    const char *relative = "car_engine.wav";
    auto path = reinterpret_cast<jstring>(const_cast<char *>(relative));
    for (int i = 0; i < 1000; ++i) {
        invoke(preloadEffect, path);
        invoke(preloadEffect, path);
        assert(SoLoud::Wav::live == 1);
        assert(gSoundManager.sounds.size() == 1);
        gSoundManager.handles["ux0:/data/hcr/assets/car_engine.wav"] = {23, 42};
        invoke(unloadEffect, path);
        assert(gSoundManager.sounds.empty());
        assert(gSoundManager.handles.empty());
        assert(SoLoud::Wav::live == 0);
        assert(gAudioSystem.soloud.stopped.size() == static_cast<size_t>((i + 1) * 2));
        invoke(unloadEffect, path);
    }
    invoke(unloadEffect, static_cast<jstring>(nullptr));
    assert(gSoundManager.sounds.empty());
}
