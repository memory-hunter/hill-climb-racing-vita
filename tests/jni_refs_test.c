#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_ImplBridge.h>

static size_t live_allocations;
static int fail_after = -1;
static unsigned original_global_deletes;

static void *test_malloc(size_t size) {
    if (fail_after == 0) return NULL;
    if (fail_after > 0) --fail_after;
    void *value = malloc(size ? size : 1);
    if (value) ++live_allocations;
    return value;
}

static void test_free(void *value) {
    if (value) {
        assert(live_allocations != 0);
        --live_allocations;
        free(value);
    }
}

static JavaDynArray *test_array(void) {
    JavaDynArray *array = test_malloc(sizeof(*array));
    assert(array);
    array->array = test_malloc(16);
    assert(array->array);
    return array;
}

jboolean jda_free(JavaDynArray *array) {
    if (!array) return JNI_FALSE;
    test_free(array->array);
    test_free(array);
    return JNI_TRUE;
}

static jclass test_find_class(JNIEnv *env, const char *name) {
    (void)env;
    char *value = test_malloc(strlen(name) + 1);
    if (value) strcpy(value, name);
    return value;
}

static jstring test_new_string_utf(JNIEnv *env, const char *bytes) {
    (void)env;
    (void)bytes;
    JavaString *value = test_malloc(sizeof(*value));
    assert(value);
    value->utf8 = test_array();
    value->utf16 = test_array();
    return value;
}

static jstring test_new_string(JNIEnv *env, const jchar *chars, jsize count) {
    (void)chars;
    (void)count;
    JavaString *value = test_new_string_utf(env, "");
    jda_free(value->utf8);
    value->utf8 = NULL;
    return value;
}

static void ignored_delete(JNIEnv *env, jobject obj) { (void)env; (void)obj; }
static void test_delete_global(JNIEnv *env, jobject obj) {
    (void)env;
    if (obj == (jobject)(uintptr_t)0x42424242 || obj == (jobject)(uintptr_t)0x69696969) return;
    ++original_global_deletes;
    test_free(obj);
}
static jobject identity_ref(JNIEnv *env, jobject obj) { (void)env; return obj; }
static struct JNINativeInterface table = {
    .FindClass = test_find_class,
    .NewStringUTF = test_new_string_utf,
    .NewString = test_new_string,
    .DeleteLocalRef = ignored_delete,
    .DeleteGlobalRef = test_delete_global,
    .NewLocalRef = identity_ref,
    .NewGlobalRef = identity_ref,
};
JNIEnv jni = &table;

#define malloc test_malloc
#define free test_free
#include "../source/reimpl/jni_refs.c"
#undef malloc
#undef free

int main(void) {
    jni_refs_init();
    jni_refs_init();
    for (int i = 0; i < 10000; ++i) {
        jclass clazz = jni->FindClass(&jni, "org/cocos2dx/lib/Cocos2dxHelper");
        assert(clazz);
        jni->DeleteLocalRef(&jni, clazz);
        jstring value = jni->NewStringUTF(&jni, "engine.wav");
        jni->DeleteLocalRef(&jni, value);
        assert(live_allocations == 0);
    }

    jstring value = jni->NewString(&jni, NULL, 0);
    jobject global = jni->NewGlobalRef(&jni, value);
    jobject local = jni->NewLocalRef(&jni, value);
    jni->DeleteLocalRef(&jni, value);
    assert(live_allocations != 0);
    jni->DeleteGlobalRef(&jni, global);
    assert(live_allocations != 0);
    jni->DeleteLocalRef(&jni, local);
    assert(live_allocations == 0);

    value = jni->NewStringUTF(&jni, "retained");
    global = jni->NewGlobalRef(&jni, value);
    jni->DeleteGlobalRef(&jni, global);
    assert(live_allocations != 0);
    jni->DeleteLocalRef(&jni, value);
    assert(live_allocations == 0);

    jni->DeleteLocalRef(&jni, NULL);
    jni->DeleteGlobalRef(&jni, (jobject)(uintptr_t)0x42424242);
    assert(jni->NewLocalRef(&jni, (jobject)(uintptr_t)0x69696969) == (jobject)(uintptr_t)0x69696969);
    assert(live_allocations == 0);

    jobject untracked = test_malloc(8);
    assert(untracked);
    jni->DeleteLocalRef(&jni, untracked);
    assert(live_allocations == 1);
    assert(jni->NewGlobalRef(&jni, untracked) == untracked);
    jni->DeleteGlobalRef(&jni, untracked);
    assert(original_global_deletes == 1);
    assert(live_allocations == 0);

    fail_after = 1;
    assert(jni->FindClass(&jni, "allocation/failure") == NULL);
    fail_after = -1;
    assert(live_allocations == 0);
}
