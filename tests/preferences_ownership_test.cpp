#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <unordered_set>
#include <unistd.h>

#define DATA_PATH "unused-test-path/"
#include "../source/SharedPreferences.cpp"

struct MockString : _jstring {
    std::string value;
    unsigned references = 1;
    explicit MockString(const char *text) : value(text) {}
};
static std::unordered_set<MockString *> live;
static JNINativeInterface table = {};
JNIEnv jni = {&table};
extern "C" void _log_print(int, const char *, ...) {}
extern "C" bool file_copy(const char *, const char *) { return true; }

static MockString *checked(jstring string) {
    auto *value = static_cast<MockString *>(string);
    assert(live.contains(value));
    return value;
}

static void invoke(void (*method)(jmethodID, va_list), ...) {
    va_list args;
    va_start(args, method);
    method(nullptr, args);
    va_end(args);
}

static jobject fetch(jobject (*method)(jmethodID, va_list), ...) {
    va_list args;
    va_start(args, method);
    jobject result = method(nullptr, args);
    va_end(args);
    return result;
}

int main() {
    table.NewStringUTF = [](JNIEnv *, const char *text) -> jstring {
        auto *value = new MockString(text);
        live.insert(value);
        return value;
    };
    table.GetStringUTFChars = [](JNIEnv *, jstring string, jboolean *) -> const char * {
        return strdup(checked(string)->value.c_str());
    };
    table.GetStringLength = [](JNIEnv *, jstring string) -> jsize {
        return checked(string)->value.size();
    };
    table.ReleaseStringUTFChars = [](JNIEnv *, jstring, char *text) { free(text); };
    table.NewLocalRef = [](JNIEnv *, jobject object) -> jobject {
        if (object) ++checked(static_cast<jstring>(object))->references;
        return object;
    };
    table.DeleteLocalRef = [](JNIEnv *, jobject object) {
        if (!object) return;
        auto *value = checked(static_cast<jstring>(object));
        if (--value->references == 0) {
            live.erase(value);
            delete value;
        }
    };

    jstring key = jni.NewStringUTF("driver");
    jstring fallback = jni.NewStringUTF("default");
    auto missing = fetch(getStringForKey, key, fallback);
    jni.DeleteLocalRef(missing);
    assert(checked(fallback)->value == "default");

    for (int i = 0; i < 1000; ++i) {
        jstring value = jni.NewStringUTF("saved value");
        invoke(setStringForKey, key, value);
        jni.DeleteLocalRef(value);
        jstring result = static_cast<jstring>(fetch(getStringForKey, key, fallback));
        assert(checked(result)->value == "saved value");
        jni.DeleteLocalRef(result);
        assert(live.size() == 2);
    }

    char path[] = "/tmp/hcr-prefs-test-XXXXXX";
    int fd = mkstemp(path);
    assert(fd >= 0);
    close(fd);
    savePrefs(path);
    loadPrefs(path);
    unlink(path);
    jstring restored = static_cast<jstring>(fetch(getStringForKey, key, fallback));
    assert(checked(restored)->value == "saved value");
    jni.DeleteLocalRef(restored);
    jni.DeleteLocalRef(key);
    jni.DeleteLocalRef(fallback);
    prefsStrings.clear();
    assert(live.empty());
}
