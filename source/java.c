#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>

/*
 * Hill Climb Racing methods
 */
void setAnimationInterval(jmethodID id, va_list args)
{
	jdouble interval = va_arg(args, jdouble);
	fjni_logv_dbg("[FalsoJNI] setAnimationInterval called with %f", (float)interval);
}

jint getIntegerForKey(jmethodID id, va_list args)
{
	jstring key = va_arg(args, jstring);
	jint defaultValue = va_arg(args, jint);
	const char *k = jni->GetStringUTFChars(&jni, key, NULL);
	fjni_logv_dbg("[FalsoJNI] getIntegerForKey(%s, %d) called — returning default", k, defaultValue);
	jni->ReleaseStringUTFChars(&jni, key, k);
	return defaultValue;
}

jstring getCocos2dxPackageName(jmethodID id, va_list args)
{
	fjni_logv_dbg("%s", "[FalsoJNI] getCocos2dxPackageName() called");
	return jni->NewStringUTF(&jni, "com.fingersoft.hillclimb");
}

/*
 * JNI Methods
 */

// start functions from id 100
NameToMethodID nameToMethodId[] = {
	{100, "setAnimationInterval", METHOD_TYPE_VOID},
	{101, "getIntegerForKey", METHOD_TYPE_INT},
	{102, "getCocos2dxPackageName", METHOD_TYPE_OBJECT},
};

MethodsBoolean methodsBoolean[] = {};
MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {};
MethodsInt methodsInt[] = {
	{101, getIntegerForKey},
};
MethodsLong methodsLong[] = {};
MethodsObject methodsObject[] = {
	{102, getCocos2dxPackageName},
};
MethodsShort methodsShort[] = {};
MethodsVoid methodsVoid[] = {
	{100, setAnimationInterval},
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