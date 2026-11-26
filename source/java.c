#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>
#include <stdint.h>
#include <psp2/kernel/processmgr.h>

#include "SharedPreferences.h"
#include "audio.h"

/*
 * Hill Climb Racing methods
 */
void setAnimationInterval(jmethodID id, va_list args)
{
	jdouble interval = va_arg(args, jdouble);
	fjni_logv_info("[FalsoJNI] setAnimationInterval(%f) called", (float)interval);
}

jstring getCocos2dxPackageName(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getCocos2dxPackageName() called");
	return jni->NewStringUTF(&jni, "hcr");
}

jint getMarketVariation(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getMarketVariation() called");
	return 0;
}

jstring getDeviceLanguage(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getDeviceLanguage() called");
	return jni->NewStringUTF(&jni, "en");
}

jstring getAndroidVersion(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getAndroidVersion() called");
	return jni->NewStringUTF(&jni, "4.4");
}

jint getSettingInt(jmethodID id, va_list args)
{
	jstring key = va_arg(args, jstring);
	jint defaultValue = va_arg(args, jint);
	fjni_logv_info("[FalsoJNI] getSettingInt(%s, %d) called", key, defaultValue);
	return defaultValue;
}

jstring getSettingString(jmethodID id, va_list args)
{
	va_arg(args, jstring);
	jstring defaultValue = va_arg(args, jstring);
	fjni_logv_info("%s", "[FalsoJNI] getSettingString() called");
	return jni->NewLocalRef(&jni, defaultValue);
}

jstring getFilesDirectory(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getFilesDirectory() called");
	return jni->NewStringUTF(&jni, DATA_PATH "files");
}

jboolean isTv(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] isTv() called");
	return JNI_FALSE;
}

jboolean isScientificRevenueEnabled(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] isScientificRevenueEnabled() called");
	return JNI_FALSE;
}

void unlockAchievement(jmethodID id, va_list args)
{
	va_arg(args, jstring);
	fjni_logv_info("%s", "[FalsoJNI] unlockAchievement() called");
}

void debugStringOnAndroid(jmethodID id, va_list args)
{
	va_arg(args, jstring);
	fjni_logv_info("%s", "[FalsoJNI] debugStringOnAndroid() called");
}

void onFinishedAssetLoader(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] onFinishedAssetLoader() called");
}

jint hasVideoCampaigns(jmethodID id, va_list args)
{
	va_arg(args, jint);
	fjni_logv_info("%s", "[FalsoJNI] hasVideoCampaigns() called");
	return 0;
}

jboolean hasEulaBeenAccepted(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] hasEulaBeenAccepted() called");
	return JNI_FALSE;
}

jint getIAPGems(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getIAPGems() called");
	return 0;
}

jint getIAPBundle(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getIAPBundle() called");
	return 0;
}

jint getIAPError(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getIAPError() called");
	return 0;
}

void showEULA(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] showEULA() called");
}

void trackEvent(jmethodID id, va_list args)
{
	va_arg(args, jstring);
	va_arg(args, jobject);
	fjni_logv_info("%s", "[FalsoJNI] trackEvent() called");
}

void disableAdManagerPopupDialogs(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] disableAdManagerPopupDialogs() called");
}

void enterInGame(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] enterInGame() called");
}

jboolean isGoogleSignedIn(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] isGoogleSignedIn() called");
	return JNI_FALSE;
}

void suggestAppRating(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] suggestAppRating() called");
}

void leaveInGame(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] leaveInGame() called");
}

jobject hashMapConstructor(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] HashMap constructor called");
	// FalsoJNI reserves this value for non-owning fake Java objects.
	return (jobject)(uintptr_t)0x42424242;
}

jobject hashMapPut(jmethodID id, va_list args)
{
	va_arg(args, jobject);
	va_arg(args, jobject);
	fjni_logv_info("%s", "[FalsoJNI] HashMap.put() called");
	return NULL;
}

void showInterstitialPromo(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] showInterstitialPromo() called");
}

void showCrossPromotion(jmethodID id, va_list args)
{
	va_arg(args, jint);
	fjni_logv_info("%s", "[FalsoJNI] showCrossPromotion() called");
}

void setAchievementSteps(jmethodID id, va_list args)
{
	va_arg(args, jstring);
	va_arg(args, jint);
	fjni_logv_info("%s", "[FalsoJNI] setAchievementSteps() called");
}

void flush(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] flush() called");
}

jboolean hasInstallReward(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] flush() called");
	return JNI_FALSE;
}

jint getIAPCoins(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getIAPCoins() called");
	return 0;
}

jint getIAPAdFree(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getIAPAdFree() called");
	return 0;
}

jboolean hasValue(jmethodID id, va_list args)
{
	jstring key = va_arg(args, jstring);
	fjni_logv_info("[FalsoJNI] hasValue(%s) called", key);
	return JNI_FALSE;
}

void startAdView(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] startAdView() called");
}

void trackPage(jmethodID id, va_list args)
{
	jstring arg1 = va_arg(args, jstring);
	fjni_logv_info("[FalsoJNI] trackPage(%s) called", arg1);
}

void stopAdView(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] stopAdView() called");
}

jint getApiLevel(jmethodID id, va_list args)
{
	fjni_logv_info("%s", "[FalsoJNI] getApiLevel() called");
	return 19;
}

void terminateProcess(jmethodID id, va_list args)
{
	_savePrefs();
	sceKernelExitProcess(0);
}

/*
 * JNI Methods
 */

// start functions from id 100
NameToMethodID nameToMethodId[] = {
	{100, "setAnimationInterval", METHOD_TYPE_VOID},
	{101, "getIntegerForKey", METHOD_TYPE_INT},
	{102, "getCocos2dxPackageName", METHOD_TYPE_OBJECT},
	{103, "getMarketVariation", METHOD_TYPE_INT},
	{104, "getDeviceLanguage", METHOD_TYPE_OBJECT},
	{105, "getAndroidVersion", METHOD_TYPE_OBJECT},
	{109, "getSettingInt", METHOD_TYPE_INT},
	{110, "setStringForKey", METHOD_TYPE_VOID},
	{111, "flush", METHOD_TYPE_VOID},
	{112, "getStringForKey", METHOD_TYPE_OBJECT},
	{113, "setIntegerForKey", METHOD_TYPE_VOID},
	{114, "hasInstallReward", METHOD_TYPE_BOOLEAN},
	{115, "getIAPCoins", METHOD_TYPE_INT},
	{116, "getIAPAdFree", METHOD_TYPE_INT},
	{117, "hasValue", METHOD_TYPE_BOOLEAN},
	{118, "startAdView", METHOD_TYPE_VOID},
	{119, "stopAdView", METHOD_TYPE_VOID},
	{120, "trackPage", METHOD_TYPE_VOID},
	{121, "getApiLevel", METHOD_TYPE_INT},
	// sound shit
	{108, "preloadEffect", METHOD_TYPE_VOID},
	{122, "unloadEffect", METHOD_TYPE_VOID},
	{123, "playEffect", METHOD_TYPE_INT},
	{124, "setEffectVolume", METHOD_TYPE_VOID},
	{125, "setEffectRate", METHOD_TYPE_VOID},
	{126, "stopEffect", METHOD_TYPE_VOID},
	{127, "pauseEffect", METHOD_TYPE_VOID},
	{128, "resumeEffect", METHOD_TYPE_VOID},
	{129, "pauseAllEffects", METHOD_TYPE_VOID},
	{130, "resumeAllEffects", METHOD_TYPE_VOID},
	{131, "stopAllEffects", METHOD_TYPE_VOID},
	{132, "getEffectsVolume", METHOD_TYPE_FLOAT},
	{133, "setEffectsVolume", METHOD_TYPE_VOID},
	{134, "preloadEffect", METHOD_TYPE_VOID},
	// music shit
	{200, "preloadBackgroundMusic", METHOD_TYPE_VOID},
	{201, "playBackgroundMusic", METHOD_TYPE_VOID},
	{202, "stopBackgroundMusic", METHOD_TYPE_VOID},
	{203, "pauseBackgroundMusic", METHOD_TYPE_VOID},
	{204, "resumeBackgroundMusic", METHOD_TYPE_VOID},
	{205, "rewindBackgroundMusic", METHOD_TYPE_VOID},
	{206, "isBackgroundMusicPlaying", METHOD_TYPE_BOOLEAN},
	{207, "endBackgroundMusic", METHOD_TYPE_VOID},
	{208, "getBackgroundVolume", METHOD_TYPE_FLOAT},
	{209, "setBackgroundMusicVolume", METHOD_TYPE_VOID},
	{210, "getFilesDirectory", METHOD_TYPE_OBJECT},
	{211, "isTv", METHOD_TYPE_BOOLEAN},
	{212, "isScientificRevenueEnabled", METHOD_TYPE_BOOLEAN},
	{213, "unlockAchievement", METHOD_TYPE_VOID},
	{214, "getSettingString", METHOD_TYPE_OBJECT},
	{215, "debugStringOnAndroid", METHOD_TYPE_VOID},
	{216, "onFinishedAssetLoader", METHOD_TYPE_VOID},
	{217, "hasVideoCampaigns", METHOD_TYPE_INT},
	{218, "hasEulaBeenAccepted", METHOD_TYPE_BOOLEAN},
	{219, "getIAPGems", METHOD_TYPE_INT},
	{220, "getIAPBundle", METHOD_TYPE_INT},
	{221, "getIAPError", METHOD_TYPE_INT},
	{222, "showEULA", METHOD_TYPE_VOID},
	{223, "trackEvent", METHOD_TYPE_VOID},
	{224, "disableAdManagerPopupDialogs", METHOD_TYPE_VOID},
	{225, "enterInGame", METHOD_TYPE_VOID},
	{226, "isGoogleSignedIn", METHOD_TYPE_BOOLEAN},
	{227, "suggestAppRating", METHOD_TYPE_VOID},
	{228, "leaveInGame", METHOD_TYPE_VOID},
	{229, "java/util/HashMap/<init>", METHOD_TYPE_OBJECT},
	{230, "put", METHOD_TYPE_OBJECT},
	{231, "showInterstitialPromo", METHOD_TYPE_VOID},
	{232, "showCrossPromotion", METHOD_TYPE_VOID},
	{233, "setAchievementSteps", METHOD_TYPE_VOID},
	{300, "terminateProcess", METHOD_TYPE_VOID},
};

MethodsBoolean methodsBoolean[] = {
	{114, hasInstallReward},
	{117, hasValue},
	{206, isBackgroundMusicPlaying},
	{211, isTv},
	{212, isScientificRevenueEnabled},
	{218, hasEulaBeenAccepted},
	{226, isGoogleSignedIn},
};
MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {
	{132, getEffectsVolume},
	{208, getBackgroundVolume},
};
MethodsInt methodsInt[] = {
	{101, getIntegerForKey},
	{103, getMarketVariation},
	{109, getSettingInt},
	{115, getIAPCoins},
	{116, getIAPAdFree},
	{121, getApiLevel},
	{123, playEffect},
	{217, hasVideoCampaigns},
	{219, getIAPGems},
	{220, getIAPBundle},
	{221, getIAPError},
};

MethodsLong methodsLong[] = {};
MethodsObject methodsObject[] = {
	{102, getCocos2dxPackageName},
	{104, getDeviceLanguage},
	{105, getAndroidVersion},
	{112, getStringForKey},
	{210, getFilesDirectory},
	{214, getSettingString},
	{229, hashMapConstructor},
	{230, hashMapPut},
};
MethodsShort methodsShort[] = {};
MethodsVoid methodsVoid[] = {
	{108, preloadEffect},
	{100, setAnimationInterval},
	{110, setStringForKey},
	{111, flush},
	{113, setIntegerForKey},
	{118, startAdView},
	{119, stopAdView},
	{120, trackPage},
	{122, unloadEffect},
	{124, setEffectVolume},
	{125, setEffectRate},
	{126, stopEffect},
	{127, pauseEffect},
	{128, resumeEffect},
	{129, pauseAllEffects},
	{130, resumeAllEffects},
	{131, stopAllEffects},
	{133, setEffectsVolume},
	{200, preloadBackgroundMusic},
	{201, playBackgroundMusic},
	{202, stopBackgroundMusic},
	{203, pauseBackgroundMusic},
	{204, resumeBackgroundMusic},
	{205, rewindBackgroundMusic},
	{207, endBackgroundMusic},
	{209, setBackgroundVolume},
	{213, unlockAchievement},
	{215, debugStringOnAndroid},
	{216, onFinishedAssetLoader},
	{222, showEULA},
	{223, trackEvent},
	{224, disableAdManagerPopupDialogs},
	{225, enterInGame},
	{227, suggestAppRating},
	{228, leaveInGame},
	{231, showInterstitialPromo},
	{232, showCrossPromotion},
	{233, setAchievementSteps},
	{300, terminateProcess},
};

/*
 * JNI Fields
 */

// System-wide constant that applications sometimes request
// https://developer.android.com/reference/android/content/Context.html#WINDOW_SERVICE
char WINDOW_SERVICE[] = "window";

// System-wide constant that's often used to determine Android version
// https://developer.android.com/reference/android/os/Build.VERSION.html#SDK_INT
// Possible values: https://developer.android.com/reference/android/os/Build.VERSION_CODES
const int SDK_INT = 19; // Android 4.4 / KitKat

NameToFieldID nameToFieldId[] = {
	{0, "WINDOW_SERVICE", FIELD_TYPE_OBJECT},
	{1, "SDK_INT", FIELD_TYPE_INT},
};

FieldsBoolean fieldsBoolean[] = {};
FieldsByte fieldsByte[] = {};
FieldsChar fieldsChar[] = {};
FieldsDouble fieldsDouble[] = {};
FieldsFloat fieldsFloat[] = {};
FieldsInt fieldsInt[] = {
	{1, SDK_INT},
};
FieldsObject fieldsObject[] = {
	{0, WINDOW_SERVICE},
};
FieldsLong fieldsLong[] = {};
FieldsShort fieldsShort[] = {};

__FALSOJNI_IMPL_CONTAINER_SIZES
