#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_Impl.h>

static unsigned local_ref_calls;
static unsigned new_string_calls;

static jobject test_new_local_ref(JNIEnv *env, jobject object) {
    (void)env;
    ++local_ref_calls;
    return object;
}

static jstring test_new_string_utf(JNIEnv *env, const char *text) {
    (void)env;
    ++new_string_calls;
    return (jstring)strdup(text);
}

static struct JNINativeInterface table = {
    .NewLocalRef = test_new_local_ref,
    .NewStringUTF = test_new_string_utf,
};
JNIEnv jni = &table;

void _fjni_log_info(const char *file, int line, const char *function, const char *format, ...) {
    (void)file;
    (void)line;
    (void)function;
    (void)format;
}

void _fjni_log_warn(const char *file, int line, const char *function, const char *format, ...) {
    (void)file;
    (void)line;
    (void)function;
    (void)format;
}

void _fjni_log_debug(const char *file, int line, const char *function, const char *format, ...) {
    (void)file;
    (void)line;
    (void)function;
    (void)format;
}

void _fjni_log_error(const char *file, int line, const char *function, const char *format, ...) {
    (void)file;
    (void)line;
    (void)function;
    (void)format;
}

#define STUB_VOID(name) \
    void name(jmethodID id, va_list args) { (void)id; (void)args; }

#define STUB_BOOLEAN(name) \
    jboolean name(jmethodID id, va_list args) { (void)id; (void)args; return JNI_FALSE; }

#define STUB_INT(name) \
    jint name(jmethodID id, va_list args) { (void)id; (void)args; return 0; }

#define STUB_FLOAT(name) \
    jfloat name(jmethodID id, va_list args) { (void)id; (void)args; return 0.0f; }

STUB_INT(getIntegerForKey)
STUB_VOID(setIntegerForKey)
jobject getStringForKey(jmethodID id, va_list args) { (void)id; (void)args; return NULL; }
STUB_VOID(setStringForKey)
STUB_VOID(preloadEffect)
STUB_VOID(unloadEffect)
STUB_INT(playEffect)
STUB_VOID(setEffectVolume)
STUB_VOID(setEffectRate)
STUB_VOID(stopEffect)
STUB_VOID(pauseEffect)
STUB_VOID(resumeEffect)
STUB_VOID(pauseAllEffects)
STUB_VOID(resumeAllEffects)
STUB_VOID(stopAllEffects)
STUB_FLOAT(getEffectsVolume)
STUB_VOID(setEffectsVolume)
STUB_VOID(preloadBackgroundMusic)
STUB_VOID(playBackgroundMusic)
STUB_VOID(stopBackgroundMusic)
STUB_VOID(pauseBackgroundMusic)
STUB_VOID(resumeBackgroundMusic)
STUB_VOID(rewindBackgroundMusic)
STUB_BOOLEAN(isBackgroundMusicPlaying)
STUB_VOID(endBackgroundMusic)
STUB_FLOAT(getBackgroundVolume)
STUB_VOID(setBackgroundVolume)

#define DATA_PATH "ux0:data/hcr/"
#include "../source/java.c"

static NameToMethodID *find_method(const char *name) {
    size_t count = nameToMethodId_size() / sizeof(nameToMethodId[0]);
    for (size_t i = 0; i < count; ++i) {
        if (strcmp(nameToMethodId[i].name, name) == 0) return &nameToMethodId[i];
    }
    return NULL;
}

static MethodsBoolean *find_boolean(int id) {
    size_t count = methodsBoolean_size() / sizeof(methodsBoolean[0]);
    for (size_t i = 0; i < count; ++i) {
        if (methodsBoolean[i].id == id) return &methodsBoolean[i];
    }
    return NULL;
}

static MethodsObject *find_object(int id) {
    size_t count = methodsObject_size() / sizeof(methodsObject[0]);
    for (size_t i = 0; i < count; ++i) {
        if (methodsObject[i].id == id) return &methodsObject[i];
    }
    return NULL;
}

static MethodsVoid *find_void(int id) {
    size_t count = methodsVoid_size() / sizeof(methodsVoid[0]);
    for (size_t i = 0; i < count; ++i) {
        if (methodsVoid[i].id == id) return &methodsVoid[i];
    }
    return NULL;
}

static MethodsInt *find_int(int id) {
    size_t count = methodsInt_size() / sizeof(methodsInt[0]);
    for (size_t i = 0; i < count; ++i) {
        if (methodsInt[i].id == id) return &methodsInt[i];
    }
    return NULL;
}

static jobject invoke_object(MethodsObject *method, ...) {
    va_list args;
    va_start(args, method);
    jobject result = method->Method((jmethodID)(uintptr_t)method->id, args);
    va_end(args);
    return result;
}

static jboolean invoke_boolean(MethodsBoolean *method, ...) {
    va_list args;
    va_start(args, method);
    jboolean result = method->Method((jmethodID)(uintptr_t)method->id, args);
    va_end(args);
    return result;
}

static void invoke_void(MethodsVoid *method, ...) {
    va_list args;
    va_start(args, method);
    method->Method((jmethodID)(uintptr_t)method->id, args);
    va_end(args);
}

static jint invoke_int(MethodsInt *method, ...) {
    va_list args;
    va_start(args, method);
    jint result = method->Method((jmethodID)(uintptr_t)method->id, args);
    va_end(args);
    return result;
}

int main(void) {
    NameToMethodID *files = find_method("getFilesDirectory");
    NameToMethodID *tv = find_method("isTv");
    NameToMethodID *scientific = find_method("isScientificRevenueEnabled");
    NameToMethodID *achievement = find_method("unlockAchievement");
    NameToMethodID *setting = find_method("getSettingString");
    NameToMethodID *debug = find_method("debugStringOnAndroid");
    NameToMethodID *finished = find_method("onFinishedAssetLoader");
    NameToMethodID *campaigns = find_method("hasVideoCampaigns");
    NameToMethodID *eula = find_method("hasEulaBeenAccepted");
    NameToMethodID *gems = find_method("getIAPGems");
    NameToMethodID *bundle = find_method("getIAPBundle");
    NameToMethodID *iap_error = find_method("getIAPError");
    NameToMethodID *show_eula = find_method("showEULA");
    NameToMethodID *track_event = find_method("trackEvent");
    NameToMethodID *disable_popups = find_method("disableAdManagerPopupDialogs");
    NameToMethodID *enter_game = find_method("enterInGame");
    NameToMethodID *google_signed_in = find_method("isGoogleSignedIn");
    NameToMethodID *suggest_rating = find_method("suggestAppRating");
    NameToMethodID *leave_game = find_method("leaveInGame");
    NameToMethodID *hash_map_constructor = find_method("java/util/HashMap/<init>");
    NameToMethodID *hash_map_put = find_method("put");
    NameToMethodID *interstitial = find_method("showInterstitialPromo");
    NameToMethodID *cross_promotion = find_method("showCrossPromotion");
    NameToMethodID *achievement_steps = find_method("setAchievementSteps");

    assert(files && files->f == METHOD_TYPE_OBJECT);
    assert(tv && tv->f == METHOD_TYPE_BOOLEAN);
    assert(scientific && scientific->f == METHOD_TYPE_BOOLEAN);
    assert(achievement && achievement->f == METHOD_TYPE_VOID);
    assert(setting && setting->f == METHOD_TYPE_OBJECT);
    assert(debug && debug->f == METHOD_TYPE_VOID);
    assert(finished && finished->f == METHOD_TYPE_VOID);
    assert(campaigns && campaigns->f == METHOD_TYPE_INT);
    assert(eula && eula->f == METHOD_TYPE_BOOLEAN);
    assert(gems && gems->f == METHOD_TYPE_INT);
    assert(bundle && bundle->f == METHOD_TYPE_INT);
    assert(iap_error && iap_error->f == METHOD_TYPE_INT);
    assert(show_eula && show_eula->f == METHOD_TYPE_VOID);
    assert(track_event && track_event->f == METHOD_TYPE_VOID);
    assert(disable_popups && disable_popups->f == METHOD_TYPE_VOID);
    assert(enter_game && enter_game->f == METHOD_TYPE_VOID);
    assert(google_signed_in && google_signed_in->f == METHOD_TYPE_BOOLEAN);
    assert(suggest_rating && suggest_rating->f == METHOD_TYPE_VOID);
    assert(leave_game && leave_game->f == METHOD_TYPE_VOID);
    assert(hash_map_constructor && hash_map_constructor->f == METHOD_TYPE_OBJECT);
    assert(hash_map_put && hash_map_put->f == METHOD_TYPE_OBJECT);
    assert(interstitial && interstitial->f == METHOD_TYPE_VOID);
    assert(cross_promotion && cross_promotion->f == METHOD_TYPE_VOID);
    assert(achievement_steps && achievement_steps->f == METHOD_TYPE_VOID);

    MethodsObject *files_impl = find_object(files->id);
    MethodsObject *setting_impl = find_object(setting->id);
    MethodsBoolean *tv_impl = find_boolean(tv->id);
    MethodsBoolean *scientific_impl = find_boolean(scientific->id);
    MethodsVoid *achievement_impl = find_void(achievement->id);
    MethodsVoid *debug_impl = find_void(debug->id);
    MethodsVoid *finished_impl = find_void(finished->id);
    MethodsInt *campaigns_impl = find_int(campaigns->id);
    MethodsBoolean *eula_impl = find_boolean(eula->id);
    MethodsInt *gems_impl = find_int(gems->id);
    MethodsInt *bundle_impl = find_int(bundle->id);
    MethodsInt *iap_error_impl = find_int(iap_error->id);
    MethodsVoid *show_eula_impl = find_void(show_eula->id);
    MethodsVoid *track_event_impl = find_void(track_event->id);
    MethodsVoid *disable_popups_impl = find_void(disable_popups->id);
    MethodsVoid *enter_game_impl = find_void(enter_game->id);
    MethodsBoolean *google_signed_in_impl = find_boolean(google_signed_in->id);
    MethodsVoid *suggest_rating_impl = find_void(suggest_rating->id);
    MethodsVoid *leave_game_impl = find_void(leave_game->id);
    MethodsObject *hash_map_constructor_impl = find_object(hash_map_constructor->id);
    MethodsObject *hash_map_put_impl = find_object(hash_map_put->id);
    MethodsVoid *interstitial_impl = find_void(interstitial->id);
    MethodsVoid *cross_promotion_impl = find_void(cross_promotion->id);
    MethodsVoid *achievement_steps_impl = find_void(achievement_steps->id);
    assert(files_impl && setting_impl && tv_impl && scientific_impl && achievement_impl);
    assert(debug_impl && finished_impl && campaigns_impl && eula_impl);
    assert(gems_impl && bundle_impl && iap_error_impl);
    assert(show_eula_impl && track_event_impl && disable_popups_impl && enter_game_impl);
    assert(google_signed_in_impl);
    assert(suggest_rating_impl && leave_game_impl);
    assert(hash_map_constructor_impl && hash_map_put_impl);
    assert(interstitial_impl && cross_promotion_impl && achievement_steps_impl);

    jstring directory = (jstring)invoke_object(files_impl);
    assert(strcmp((const char *)directory, "ux0:data/hcr/files") == 0);
    assert(new_string_calls == 1);
    free(directory);

    jstring fallback = (jstring)"fallback";
    assert(invoke_object(setting_impl, (jstring)"cloud.whitelist", fallback) == fallback);
    assert(local_ref_calls == 1);
    assert(invoke_boolean(tv_impl) == JNI_FALSE);
    assert(invoke_boolean(scientific_impl) == JNI_FALSE);
    invoke_void(achievement_impl, (jstring)"CgkI-achievement");
    invoke_void(debug_impl, (jstring)"native debug output");
    invoke_void(finished_impl);
    assert(invoke_int(campaigns_impl, 17) == 0);
    assert(invoke_boolean(eula_impl) == JNI_FALSE);
    assert(invoke_int(gems_impl) == 0);
    assert(invoke_int(bundle_impl) == 0);
    assert(invoke_int(iap_error_impl) == 0);
    invoke_void(show_eula_impl);
    invoke_void(track_event_impl, (jstring)"game_start", (jobject)(uintptr_t)0x42424242);
    invoke_void(disable_popups_impl);
    invoke_void(enter_game_impl);
    assert(invoke_boolean(google_signed_in_impl) == JNI_FALSE);
    invoke_void(suggest_rating_impl);
    invoke_void(leave_game_impl);
    jobject hash_map = invoke_object(hash_map_constructor_impl);
    assert(hash_map == (jobject)(uintptr_t)0x42424242);
    assert(invoke_object(hash_map_put_impl, (jobject)"key", (jobject)"value") == NULL);
    invoke_void(interstitial_impl);
    invoke_void(cross_promotion_impl, 4);
    invoke_void(achievement_steps_impl, (jstring)"CgkI-achievement", 17);
}
