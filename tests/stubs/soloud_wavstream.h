#pragma once
#include "soloud.h"
namespace SoLoud {
struct WavStream : AudioSource {
    int load(const char *) { return 0; }
};
}
