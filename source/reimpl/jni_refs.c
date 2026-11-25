#include "jni_refs.h"

#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_ImplBridge.h>
#include <pthread.h>
#include <stdlib.h>

typedef enum { REF_CLASS, REF_STRING } RefKind;

typedef struct OwnedRef {
    jobject object;
    RefKind kind;
    size_t locals;
    size_t globals;
    struct OwnedRef *next;
} OwnedRef;

static struct JNINativeInterface original;
static OwnedRef *references;
static pthread_mutex_t reference_mutex = PTHREAD_MUTEX_INITIALIZER;

static void destroy_object(jobject object, RefKind kind) {
    if (kind == REF_STRING) {
        JavaString *string = object;
        jda_free(string->utf8);
        jda_free(string->utf16);
    }
    free(object);
}

static jobject own(jobject object, RefKind kind) {
    if (!object) return NULL;
    OwnedRef *ref = malloc(sizeof(*ref));
    if (!ref) {
        destroy_object(object, kind);
        return NULL;
    }
    *ref = (OwnedRef){.object = object, .kind = kind, .locals = 1};
    pthread_mutex_lock(&reference_mutex);
    ref->next = references;
    references = ref;
    pthread_mutex_unlock(&reference_mutex);
    return object;
}

static int retain_owned(jobject object, int global) {
    int found = 0;
    pthread_mutex_lock(&reference_mutex);
    for (OwnedRef *ref = references; ref; ref = ref->next) {
        if (ref->object != object) continue;
        if (global) ++ref->globals;
        else ++ref->locals;
        found = 1;
        break;
    }
    pthread_mutex_unlock(&reference_mutex);
    return found;
}

static int release_owned(jobject object, int global) {
    OwnedRef *dead = NULL;
    int found = 0;
    pthread_mutex_lock(&reference_mutex);
    for (OwnedRef **link = &references; *link; link = &(*link)->next) {
        OwnedRef *ref = *link;
        if (ref->object != object) continue;
        found = 1;
        size_t *count = global ? &ref->globals : &ref->locals;
        if (*count) --*count;
        if (!ref->locals && !ref->globals) {
            *link = ref->next;
            dead = ref;
        }
        break;
    }
    pthread_mutex_unlock(&reference_mutex);
    if (dead) {
        destroy_object(dead->object, dead->kind);
        free(dead);
    }
    return found;
}

static jclass find_class(JNIEnv *env, const char *name) {
    return own(original.FindClass(env, name), REF_CLASS);
}

static jstring new_string_utf(JNIEnv *env, const char *bytes) {
    return own(original.NewStringUTF(env, bytes), REF_STRING);
}

static jstring new_string(JNIEnv *env, const jchar *chars, jsize count) {
    return own(original.NewString(env, chars, count), REF_STRING);
}

static jobject new_local_ref(JNIEnv *env, jobject object) {
    return retain_owned(object, 0) ? object : original.NewLocalRef(env, object);
}

static jobject new_global_ref(JNIEnv *env, jobject object) {
    return retain_owned(object, 1) ? object : original.NewGlobalRef(env, object);
}

static void delete_local_ref(JNIEnv *env, jobject object) {
    if (!release_owned(object, 0)) original.DeleteLocalRef(env, object);
}

static void delete_global_ref(JNIEnv *env, jobject object) {
    if (!release_owned(object, 1)) original.DeleteGlobalRef(env, object);
}

void jni_refs_init(void) {
    struct JNINativeInterface *table = (struct JNINativeInterface *)jni;
    if (table->FindClass == find_class) return;
    original = *table;
    table->FindClass = find_class;
    table->NewStringUTF = new_string_utf;
    table->NewString = new_string;
    table->NewLocalRef = new_local_ref;
    table->NewGlobalRef = new_global_ref;
    // Only tracked constructors establish ownership; fake objects and external arrays stay borrowed.
    table->DeleteLocalRef = delete_local_ref;
    table->DeleteGlobalRef = delete_global_ref;
}
