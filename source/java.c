#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>

#include "SharedPreferences.h"

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

void setBackgroundMusicVolume(jmethodID id, va_list args)
{
	jdouble volume = va_arg(args, jdouble);
	fjni_logv_info("[FalsoJNI] setBackgroundMusicVolume(%f) called", volume);
}

void playBackgroundMusic(jmethodID id, va_list args)
{
	jstring path = va_arg(args, jstring);
	jint isLoop = va_arg(args, jint);
	fjni_logv_info("[FalsoJNI] playBackgroundMusic(%s, %s) called", path, isLoop ? "true" : "false");
}

void preloadEffect(jmethodID id, va_list args)
{
	jstring path = va_arg(args, jstring);
	fjni_logv_info("[FalsoJNI] preloadEffect(%s) called", path);
}

jint getSettingInt(jmethodID id, va_list args)
{
	jstring key = va_arg(args, jstring);
	jint defaultValue = va_arg(args, jint);
	fjni_logv_info("[FalsoJNI] getSettingInt(%s, %d) called", key, defaultValue);
	return defaultValue;
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

/*
 * JNI Methods
 */

// start functions from id 100
NameToMethodID nameToMethodId[] = {
	{100, "setAnimationInterval", METHOD_TYPE_VOID},
	{101, "getIntegerForKey", METHOD_TYPE_INT},
	{112, "getStringForKey", METHOD_TYPE_OBJECT},
	{113, "setIntegerForKey", METHOD_TYPE_VOID},
	{110, "setStringForKey", METHOD_TYPE_VOID},
	{102, "getCocos2dxPackageName", METHOD_TYPE_OBJECT},
	{103, "getMarketVariation", METHOD_TYPE_INT},
	{104, "getDeviceLanguage", METHOD_TYPE_OBJECT},
	{105, "getAndroidVersion", METHOD_TYPE_OBJECT},
	{106, "setBackgroundMusicVolume", METHOD_TYPE_VOID},
	{107, "playBackgroundMusic", METHOD_TYPE_VOID},
	{108, "preloadEffect", METHOD_TYPE_VOID},
	{109, "getSettingInt", METHOD_TYPE_INT},
	{111, "flush", METHOD_TYPE_VOID},
	{114, "hasInstallReward", METHOD_TYPE_BOOLEAN},
	{115, "getIAPCoins", METHOD_TYPE_INT},
	{116, "getIAPAdFree", METHOD_TYPE_INT},
	{117, "hasValue", METHOD_TYPE_BOOLEAN},
	{118, "startAdView", METHOD_TYPE_VOID},
};

MethodsBoolean methodsBoolean[] = {
	{114, hasInstallReward},
	{117, hasValue},
};
MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {};
MethodsInt methodsInt[] = {
	{101, getIntegerForKey},
	{103, getMarketVariation},
	{109, getSettingInt},
	{115, getIAPCoins},
	{116, getIAPAdFree},
};
MethodsLong methodsLong[] = {};
MethodsObject methodsObject[] = {
	{102, getCocos2dxPackageName},
	{104, getDeviceLanguage},
	{105, getAndroidVersion},
	{112, getStringForKey},
};
MethodsShort methodsShort[] = {};
MethodsVoid methodsVoid[] = {
	{100, setAnimationInterval},
	{106, setBackgroundMusicVolume},
	{107, playBackgroundMusic},
	{108, preloadEffect},
	{110, setStringForKey},
	{111, flush},
	{113, setIntegerForKey},
	{118, startAdView},
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