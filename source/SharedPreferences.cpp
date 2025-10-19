#include "SharedPreferences.h"
#include "utils/logger.h"

#include <cstdarg>
#include <cstring>
#include <map>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <falso_jni/FalsoJNI.h>
#include <falso_jni/jni.h>

std::map<std::string, bool> prefsBools;
std::map<std::string, int> prefsInts;
std::map<std::string, jstring> prefsStrings;

std::mutex prefsMutex;

const std::string shared_prefs_path = DATA_PATH "SharedPreferences.bin";

static std::thread prefsSaverThread;
static std::condition_variable prefsSaverCond;
static std::mutex prefsSaverMutex;
static std::atomic<bool> prefsSaverRunning{false};
static std::atomic<bool> needsSaving{false};
static int saveIntervalSeconds = 2;

template <typename T>
void writeValue(std::ofstream &out, const T &value)
{
    out.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

template <typename T>
void readValue(std::ifstream &in, T &value)
{
    in.read(reinterpret_cast<char *>(&value), sizeof(T));
}

void writeString(std::ofstream &out, jstring str)
{
    const char *val = jni.GetStringUTFChars(str, NULL);
    uint32_t len = static_cast<uint32_t>(jni.GetStringLength(str));
    writeValue(out, len);
    out.write(val, len);
    jni.ReleaseStringUTFChars(str, val);
}

void writeString(std::ofstream &out, const std::string &str)
{
    uint32_t len = static_cast<uint32_t>(str.size());
    writeValue(out, len);
    out.write(str.data(), len);
}

void readString(std::ifstream &in, std::string &str)
{
    uint32_t len;
    readValue(in, len);
    str.resize(len);
    in.read(&str[0], len);
}

void readString(std::ifstream &in, jstring &str)
{
    uint32_t len;
    readValue(in, len);
    std::string s(len, '\0');
    in.read(&s[0], len);
    str = jni.NewStringUTF(s.c_str());
}

void savePrefs(const std::string &filename)
{
    std::lock_guard<std::mutex> lock(prefsMutex);
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
        return;

    // bools
    writeValue(out, static_cast<uint32_t>(prefsBools.size()));
    for (auto &[k, v] : prefsBools)
    {
        writeString(out, k);
        writeValue(out, v);
    }

    // ints
    writeValue(out, static_cast<uint32_t>(prefsInts.size()));
    for (auto &[k, v] : prefsInts)
    {
        writeString(out, k);
        writeValue(out, v);
    }

    // strings
    writeValue(out, static_cast<uint32_t>(prefsStrings.size()));
    for (auto &[k, v] : prefsStrings)
    {
        writeString(out, k);
        writeString(out, v);
    }
}

void loadPrefs(const std::string &filename)
{
    std::lock_guard<std::mutex> lock(prefsMutex);
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
        return;

    prefsBools.clear();
    prefsInts.clear();
    prefsStrings.clear();

    uint32_t count;

    // bools
    readValue(in, count);
    for (uint32_t i = 0; i < count; ++i)
    {
        std::string key;
        bool val;
        readString(in, key);
        readValue(in, val);
        prefsBools[key] = val;
    }

    // ints
    readValue(in, count);
    for (uint32_t i = 0; i < count; ++i)
    {
        std::string key;
        int val;
        readString(in, key);
        readValue(in, val);
        prefsInts[key] = val;
    }

    // strings
    readValue(in, count);
    for (uint32_t i = 0; i < count; ++i)
    {
        std::string key;
        jstring val;
        readString(in, key);
        readString(in, val);
        prefsStrings[key] = val;
    }
}

jboolean getBoolForKey(jmethodID, va_list args)
{
    jstring _key = va_arg(args, jstring);
    int default_value = va_arg(args, int);

    const char *chars = jni.GetStringUTFChars(_key, nullptr);
    std::string key(chars);
    jni.ReleaseStringUTFChars(_key, chars);

    if (key == "remove_ads")
        return 1;

    std::lock_guard<std::mutex> lock(prefsMutex);
    auto it = prefsBools.find(key);
    return it != prefsBools.end() ? it->second : default_value;
}

jint getIntegerForKey(jmethodID, va_list args)
{
    jstring _key = va_arg(args, jstring);
    int default_value = va_arg(args, int);

    const char *chars = jni.GetStringUTFChars(_key, nullptr);
    std::string key(chars);
    jni.ReleaseStringUTFChars(_key, chars);

    std::lock_guard<std::mutex> lock(prefsMutex);
    auto it = prefsInts.find(key);
    return it != prefsInts.end() ? it->second : default_value;
}

jobject getStringForKey(jmethodID, va_list args)
{
    jstring _key = va_arg(args, jstring);
    jstring _default_value = va_arg(args, jstring);

    const char *chars = jni.GetStringUTFChars(_key, nullptr);
    std::string key(chars);
    jni.ReleaseStringUTFChars(_key, chars);

    std::lock_guard<std::mutex> lock(prefsMutex);
    auto it = prefsStrings.find(key);
    return it != prefsStrings.end() ? it->second : _default_value;
}

void setIntegerForKey(jmethodID, va_list args)
{
    jstring _key = va_arg(args, jstring);
    int val = va_arg(args, int);

    const char *chars = jni.GetStringUTFChars(_key, nullptr);
    std::string key(chars);
    jni.ReleaseStringUTFChars(_key, chars);

    {
        std::lock_guard<std::mutex> lock(prefsMutex);
        prefsInts[key] = val;
    }
    needsSaving.exchange(true);
}

void setBoolForKey(jmethodID, va_list args)
{
    jstring _key = va_arg(args, jstring);
    int val = va_arg(args, int);

    const char *chars = jni.GetStringUTFChars(_key, nullptr);
    std::string key(chars);
    jni.ReleaseStringUTFChars(_key, chars);

    {
        std::lock_guard<std::mutex> lock(prefsMutex);
        prefsBools[key] = val;
    }
    needsSaving.exchange(true);
}

void setStringForKey(jmethodID, va_list args)
{
    jstring _key = va_arg(args, jstring);
    jstring val = va_arg(args, jstring);

    const char *chars = jni.GetStringUTFChars(_key, nullptr);
    std::string key(chars);
    jni.ReleaseStringUTFChars(_key, chars);

    {
        std::lock_guard<std::mutex> lock(prefsMutex);
        prefsStrings[key] = val;
    }
    needsSaving.exchange(true);
}

static void prefsSaverLoop()
{
    std::unique_lock<std::mutex> lock(prefsSaverMutex);
    while (prefsSaverRunning)
    {
        prefsSaverCond.wait_for(lock, std::chrono::seconds(saveIntervalSeconds));
        if (!prefsSaverRunning)
            break;
        if (needsSaving)
        {
            l_debug("Saving preferences...");
            savePrefs(shared_prefs_path);
            needsSaving.exchange(false);
        }
    }
}

void startPrefsSaver(int intervalSec)
{
    saveIntervalSeconds = intervalSec;
    prefsSaverRunning = true;
    prefsSaverThread = std::thread(prefsSaverLoop);
}

void prefs_destroy()
{
    prefsSaverRunning = false;
    prefsSaverCond.notify_all();
    if (prefsSaverThread.joinable())
        prefsSaverThread.join();
}

void prefs_init()
{
    loadPrefs(shared_prefs_path);
    startPrefsSaver(2);
}
