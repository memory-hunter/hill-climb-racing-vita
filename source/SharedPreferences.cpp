#include "SharedPreferences.h"

#include <cstdarg>
#include <cstring>
#include <map>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <falso_jni/FalsoJNI.h>
#include <falso_jni/jni.h>

std::map<std::string, bool> prefsBools;
std::map<std::string, int> prefsInts;
std::map<std::string, jstring> prefsStrings;

std::mutex prefsMutex;

const std::string shared_prefs_path = DATA_PATH"SharedPreferences.bin";

enum class PrefType {
    Boolean,
    Integer,
    String
};

template <typename T>
void writeValue(std::ofstream &out, const T &value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template <typename T>
void readValue(std::ifstream &in, T &value) {
    in.read(reinterpret_cast<char*>(&value), sizeof(T));
}

void writeString(std::ofstream &out, jstring str) {
    const char * val = jni.GetStringUTFChars(str, NULL);
    uint32_t len = static_cast<uint32_t>(jni.GetStringLength(str));
    writeValue(out, len);
    out.write(val, len);
    jni.ReleaseStringUTFChars(str, val);
}

void writeString(std::ofstream &out, const std::string &str) {
    uint32_t len = static_cast<uint32_t>(str.size());
    writeValue(out, len);
    out.write(str.data(), len);
}

void readString(std::ifstream &in, std::string &str) {
    uint32_t len;
    readValue(in, len);
    str.resize(len);
    in.read(&str[0], len);
}

void readString(std::ifstream &in, jstring &str) {
    uint32_t len;
    readValue(in, len);

    std::string s;
    s.resize(len);
    in.read(&s[0], len);

    str = jni.NewStringUTF(s.c_str());
}

void savePrefs(PrefType type) {
    std::ofstream out("prefs.bin", std::ios::binary | std::ios::trunc);
    if (!out.is_open()) return;

    if (type == PrefType::Boolean) {
        uint32_t size = static_cast<uint32_t>(prefsBools.size());
        writeValue(out, size);
        for (auto &p : prefsBools) {
            writeString(out, p.first);
            writeValue(out, p.second);
        }
    } else if (type == PrefType::Integer) {
        uint32_t size = static_cast<uint32_t>(prefsInts.size());
        writeValue(out, size);
        for (auto &p : prefsInts) {
            writeString(out, p.first);
            writeValue(out, p.second);
        }
    } else if (type == PrefType::String) {
        uint32_t size = static_cast<uint32_t>(prefsStrings.size());
        writeValue(out, size);
        for (auto &p : prefsStrings) {
            writeString(out, p.first);
            writeString(out, p.second);
        }
    }

    out.close();
}

// save all prefs into one binary file
void savePrefs(const std::string &filename) {
    std::lock_guard<std::mutex> lock(prefsMutex);
    std::ofstream out(filename, std::ios::binary);
    if (!out) return;

    // bools
    writeValue(out, static_cast<uint32_t>(prefsBools.size()));
    for (auto &[k, v] : prefsBools) {
        writeString(out, k);
        writeValue(out, v);
    }

    // ints
    writeValue(out, static_cast<uint32_t>(prefsInts.size()));
    for (auto &[k, v] : prefsInts) {
        writeString(out, k);
        writeValue(out, v);
    }

    // strings
    writeValue(out, static_cast<uint32_t>(prefsStrings.size()));
    for (auto &[k, v] : prefsStrings) {
        writeString(out, k);
        writeString(out, v);
    }
}

void savePrefs(PrefType type, const std::string &filename) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) return;

    switch (type) {
        case PrefType::Boolean: {
            uint32_t size = static_cast<uint32_t>(prefsBools.size());
            writeValue(out, size);
            for (auto &[k, v] : prefsBools) {
                writeString(out, k);
                writeValue(out, v);
            }
            break;
        }
        case PrefType::Integer: {
            uint32_t size = static_cast<uint32_t>(prefsInts.size());
            writeValue(out, size);
            for (auto &[k, v] : prefsInts) {
                writeString(out, k);
                writeValue(out, v);
            }
            break;
        }
        case PrefType::String: {
            uint32_t size = static_cast<uint32_t>(prefsStrings.size());
            writeValue(out, size);
            for (auto &[k, v] : prefsStrings) {
                writeString(out, k);
                writeString(out, v);
            }
            break;
        }
    }
    out.close();
}

// load all prefs from one binary file
void loadPrefs(const std::string &filename) {
    std::lock_guard<std::mutex> lock(prefsMutex);
    std::ifstream in(filename, std::ios::binary);
    if (!in) return;

    prefsBools.clear();
    prefsInts.clear();
    prefsStrings.clear();

    // bools
    uint32_t count;
    readValue(in, count);
    for (uint32_t i = 0; i < count; ++i) {
        std::string key;
        bool val;
        readString(in, key);
        readValue(in, val);
        prefsBools[key] = val;
    }

    // ints
    readValue(in, count);
    for (uint32_t i = 0; i < count; ++i) {
        std::string key;
        int val;
        readString(in, key);
        readValue(in, val);
        prefsInts[key] = val;
    }

    // strings
    readValue(in, count);
    for (uint32_t i = 0; i < count; ++i) {
        std::string key;
        jstring val;
        readString(in, key);
        readString(in, val);
        prefsStrings[key] = val;
    }
}

void load_prefs() {
    loadPrefs(shared_prefs_path);
}

// async save helper
void savePrefsAsync() {
    std::string filename = shared_prefs_path;
    std::thread([filename]() {
        savePrefs(filename);
    }).detach();
}

jboolean getBoolForKey(jmethodID id, va_list args) {
    jstring _key = va_arg(args, jstring);
    int default_value = va_arg(args, int);

    const char * chars = jni.GetStringUTFChars(_key, NULL);
    auto key = std::string(jni.GetStringUTFChars(_key, NULL));
    jni.ReleaseStringUTFChars(_key, chars);

    if (key == "remove_ads")
        return 1;

    if (prefsBools.contains(key)) {
        return prefsBools[key];
    } else {
        return default_value;
    }
}

jint getIntegerForKey(jmethodID id, va_list args) {
    jstring _key = va_arg(args, jstring);
    int default_value = va_arg(args, int);

    const char * chars = jni.GetStringUTFChars(_key, NULL);
    auto key = std::string(jni.GetStringUTFChars(_key, NULL));
    jni.ReleaseStringUTFChars(_key, chars);

    if (prefsInts.contains(key)) {
        return prefsInts[key];
    } else {
        return default_value;
    }
}

jobject getStringForKey(jmethodID id, va_list args) {
    jstring _key = va_arg(args, jstring);
    jstring _default_value = va_arg(args, jstring);

    const char * chars = jni.GetStringUTFChars(_key, NULL);
    auto key = std::string(jni.GetStringUTFChars(_key, NULL));
    jni.ReleaseStringUTFChars(_key, chars);

    if (prefsStrings.contains(key)) {
        return (jobject) prefsStrings[key];
    } else {
        return (jobject) _default_value;
    }
}

void setIntegerForKey(jmethodID id, va_list args) {
    jstring _key = va_arg(args, jstring);
    int val = va_arg(args, int);

    const char * chars = jni.GetStringUTFChars(_key, NULL);
    auto key = std::string(jni.GetStringUTFChars(_key, NULL));
    jni.ReleaseStringUTFChars(_key, chars);

    prefsInts[key] = val;
    //savePrefsAsync();
    savePrefs(shared_prefs_path);
}

void setBoolForKey(jmethodID id, va_list args) {
    jstring _key = va_arg(args, jstring);
    int val = va_arg(args, int);

    const char * chars = jni.GetStringUTFChars(_key, NULL);
    auto key = std::string(jni.GetStringUTFChars(_key, NULL));
    jni.ReleaseStringUTFChars(_key, chars);

    prefsBools[key] = val;
    //savePrefsAsync();
    savePrefs(shared_prefs_path);
}

void setStringForKey(jmethodID id, va_list args) {
    jstring _key = va_arg(args, jstring);
    jstring val = va_arg(args, jstring);

    const char * chars = jni.GetStringUTFChars(_key, NULL);
    auto key = std::string(jni.GetStringUTFChars(_key, NULL));
    jni.ReleaseStringUTFChars(_key, chars);

    prefsStrings[key] = val;
    //savePrefsAsync();
    savePrefs(shared_prefs_path);
}
