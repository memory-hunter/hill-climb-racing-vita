#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>

/*
 * Hill Climb Racing methods
 */
void setAnimationInterval(jmethodID id, va_list args)
{
	jdouble interval = va_arg(args, jdouble);
	fjni_logv_info("[FalsoJNI] setAnimationInterval called with %f", (float)interval);
}

jint getIntegerForKey(jmethodID id, va_list args)
{
	jstring key = va_arg(args, jstring);
	jint defaultValue = va_arg(args, jint);
	const char *k = jni->GetStringUTFChars(&jni, key, NULL);
	fjni_logv_info("[FalsoJNI] getIntegerForKey(%s, %d) called — returning default", k, defaultValue);
	jni->ReleaseStringUTFChars(&jni, key, k);
	return defaultValue;
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
};

MethodsBoolean methodsBoolean[] = {};
MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {};
MethodsInt methodsInt[] = {
	{101, getIntegerForKey},
	{103, getMarketVariation},
};
MethodsLong methodsLong[] = {};
MethodsObject methodsObject[] = {
	{102, getCocos2dxPackageName},
	{104, getDeviceLanguage},
};
MethodsShort methodsShort[] = {};
MethodsVoid methodsVoid[] = {
	{100, setAnimationInterval},
};

/*
 * Hill Climb Racing  Fields
 */
const jlong animationInterval = 16666666LL;

/*
 * JNI Fields
 */

// System-wide constant that applications sometimes request
// https://developer.android.com/reference/android/content/Context.html#WINDOW_SERVICE
char WINDOW_SERVICE[] = "window";
char packageName[] = "ux0:/data/hcr/";

// System-wide constant that's often used to determine Android version
// https://developer.android.com/reference/android/os/Build.VERSION.html#SDK_INT
// Possible values: https://developer.android.com/reference/android/os/Build.VERSION_CODES
const int SDK_INT = 19; // Android 4.4 / KitKat

NameToFieldID nameToFieldId[] = {
	{0, "WINDOW_SERVICE", FIELD_TYPE_OBJECT},
	{1, "SDK_INT", FIELD_TYPE_INT},
	{2, "animationInterval", FIELD_TYPE_LONG},
	{3, "packageName", FIELD_TYPE_OBJECT},
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
	{3, packageName},
};
FieldsLong fieldsLong[] = {
	{2, animationInterval},
};
FieldsShort fieldsShort[] = {};

__FALSOJNI_IMPL_CONTAINER_SIZES