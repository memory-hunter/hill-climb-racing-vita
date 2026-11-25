#pragma once
#include <string>
#include <vector>

namespace SoLoud {
using handle = unsigned;
using result = int;
constexpr result SO_NO_ERROR = 0;
struct AudioSource {
    enum { SHOULD_LOOP = 1 };
    unsigned mFlags = 0;
    void setLooping(bool) {}
    void stop() {}
};
struct Soloud {
    enum { CLIP_ROUNDOFF = 1, VITA_HOMEBREW = 2 };
    std::vector<handle> stopped;
    handle next = 1;
    result init(unsigned, unsigned, unsigned, unsigned, unsigned) { return 0; }
    const char *getErrorString(result) { return "mock"; }
    void setGlobalVolume(float) {}
    void setMaxActiveVoiceCount(unsigned) {}
    void setVisualizationEnable(bool) {}
    void deinit() {}
    void stop(handle h) { stopped.push_back(h); }
    void stopAll() {}
    handle play(AudioSource &, float = 1, float = 0, bool = false, unsigned = 0) { return next++; }
    handle playBackground(AudioSource &) { return next++; }
    void setPause(handle, bool) {}
    void setPauseAll(bool) {}
    void setVolume(handle, float) {}
    void setRelativePlaySpeed(handle, float) {}
    bool isValidVoiceHandle(handle h) { return h != 0; }
    bool getPause(handle) { return false; }
};
}
