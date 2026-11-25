#pragma once
#include "soloud.h"
namespace SoLoud {
struct Wav : AudioSource {
    inline static int live = 0;
    Wav() { ++live; }
    ~Wav() { --live; }
    int load(const char *) { return 0; }
};
}
