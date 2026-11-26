#include "utils/init.h"
#include "utils/glutil.h"
#include "utils/logger.h"
#include "utils/utils.h"

#include <psp2/kernel/threadmgr.h>

#include <falso_jni/FalsoJNI.h>
#include <falso_jni/FalsoJNI_ImplBridge.h>
#include <so_util/so_util.h>

#include <pthread.h>
#include "reimpl/controls.h"
#include "SharedPreferences.h"
#include "audio.h"

int _newlib_heap_size_user = 256 * 1024 * 1024;

#ifdef USE_SCELIBC_IO
int sceLibcHeapSize = 4 * 1024 * 1024;
#endif

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define JNI_STR(s) jni->NewStringUTF(&jni, s)

so_module so_mod;

void (*Cocos2dx_nativeTouchesBegin)(JNIEnv *jni, jobject thiz, jint id, jfloat x, jfloat y);
void (*Cocos2dx_nativeTouchesMove)(JNIEnv *jni, jobject thiz, jint *ids, jfloat *xs, jfloat *ys);
void (*Cocos2dx_nativeTouchesEnd)(JNIEnv *jni, jobject thiz, jint id, jfloat x, jfloat y);
int (*Cocos2dx_nativeKeyDown)(JNIEnv *jni, jobject thiz, jint keyCode);
int (*Game_onControllerKeyEvent)(JNIEnv *jni, jobject thiz, jint keyCode, jint keyState);
void (*Game_onControllerConnectionEvent)(JNIEnv *jni, jobject thiz, jboolean connected, jint type);

JavaDynArray *touch_ids, *touch_xs, *touch_ys;

void controls_handler_key(int32_t keycode, ControlsAction action)
{
    Game_onControllerKeyEvent(&jni, NULL, keycode, action == CONTROLS_ACTION_DOWN ? 1 : 0);
}

void controls_handler_touch(int32_t id, float x, float y, ControlsAction action)
{
    ((int *)touch_ids->array)[0] = id + 8; // we use 0-7 for simulated touch events from buttons
    ((float *)touch_xs->array)[0] = x;
    ((float *)touch_ys->array)[0] = y;

    switch (action)
    {
    case CONTROLS_ACTION_DOWN:
        Cocos2dx_nativeTouchesBegin(&jni, NULL, id + 8, x, y);
        break;
    case CONTROLS_ACTION_UP:
        Cocos2dx_nativeTouchesEnd(&jni, NULL, id + 8, x, y);
        break;
    case CONTROLS_ACTION_MOVE:
        Cocos2dx_nativeTouchesMove(&jni, NULL, (jintArray)touch_ids, (jfloatArray)touch_xs, (jfloatArray)touch_ys);
        break;
    }
}

void controls_handler_analog(ControlsStickId which, float x, float y, ControlsAction action)
{
    return;
}

int main()
{
    soloader_init_all();
    prefs_init();
    controls_init();
    audio_init();
    gl_init();

    touch_ids = jda_alloc(1, FIELD_TYPE_INT);
    touch_xs = jda_alloc(1, FIELD_TYPE_FLOAT);
    touch_ys = jda_alloc(1, FIELD_TYPE_FLOAT);

    int (*JNI_OnLoad)(void *vm) = (void *)so_symbol(&so_mod, "JNI_OnLoad");
    int (*Cocos2dx_nativeSetPaths)(void *env, void *obj, jstring apkFilePath) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxActivity_nativeSetPaths");
    int (*Game_isTestingMode)() = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_isTestingMode");
    int (*Game_nativeInit)(void *env, void *obj, jint screen_width, jint screen_height) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit");
    int (*Game_nativeResize)(void *env, void *obj, jint screen_width, jint screen_height) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeResize");
    int (*Game_nativeRender)(void *env) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender");
    int (*Game_setInAppItemPrice)(void *env, void *obj, jstring item, jstring price) = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_setInAppItemPrice");
    void (*Game_setInAppItem)(void *, void *, jstring, jstring, jstring, jstring, jstring, jboolean, jboolean, jboolean, jboolean, jint) = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_setInAppItem");

    Cocos2dx_nativeTouchesBegin = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin");
    Cocos2dx_nativeTouchesMove = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove");
    Cocos2dx_nativeTouchesEnd = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd");
    Cocos2dx_nativeKeyDown = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown");
    Game_onControllerConnectionEvent = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_onControllerConnectionEvent");
    Game_onControllerKeyEvent = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_onControllerKeyEvent");

    JNI_OnLoad(&jvm);

    char sku[96], amt[16], icon[12];

    const char *tp[] = {"", "iap2.", "iap3.", "iap4."};
    long ca[4][7] = {
        {150000, 300000, 750000, 2000000, 4000000, 8000000, 20000000},
        {300000, 600000, 1500000, 4000000, 8000000, 16000000, 40000000},
        {600000, 1200000, 3000000, 8000000, 16000000, 32000000, 80000000},
        {1200000, 2400000, 6000000, 16000000, 32000000, 64000000, 160000000}};
    const char *cb[] = {"", "+25%", "+99%", "+166%", "+212%", "+308%", "+431%"};
    for (int t = 0; t < 4; t++)
        for (int i = 0; i < 7; i++)
        {
            snprintf(sku, sizeof sku, "com.fingersoft.hillclimb.%sadfree_%ldcoins", tp[t], ca[t][i]);
            snprintf(amt, sizeof amt, "%ld", ca[t][i]);
            snprintf(icon, sizeof icon, "coin%d", (t >= 2 && i == 0) ? 1 : i);
            Game_setInAppItem(&jni, NULL, JNI_STR(sku), JNI_STR("-.--"), JNI_STR(amt), JNI_STR(cb[i]), JNI_STR(icon), 0, 0, 0, 0, 0);
            Game_setInAppItemPrice(&jni, NULL, JNI_STR(sku), JNI_STR("0.00"));
        }

    const char *gp[] = {"iap2.", "iap1."};
    long ga[2][6] = {{500, 2000, 5000, 11000, 23000, 40000}, {1000, 3000, 6000, 15000, 26000, 50000}};
    const char *gb[2][6] = {{"", "+132%", "+199%", "+219%", "+305%", "+318%"},
                            {"", "+99%", "+119%", "+149%", "+172%", "+199%"}};
    for (int t = 0; t < 2; t++)
        for (int i = 0; i < 6; i++)
        {
            snprintf(sku, sizeof sku, "com.fingersoft.hillclimb.%sadfree_%ldgems", gp[t], ga[t][i]);
            snprintf(amt, sizeof amt, "%ld", ga[t][i]);
            snprintf(icon, sizeof icon, "diamond%d", i + 1);
            Game_setInAppItem(&jni, NULL, JNI_STR(sku), JNI_STR("-.--"), JNI_STR(amt), JNI_STR(gb[t][i]), JNI_STR(icon), 0, 0, 0, 1, 0);
            Game_setInAppItemPrice(&jni, NULL, JNI_STR(sku), JNI_STR("0.00"));
        }

    Game_isTestingMode();
    Cocos2dx_nativeSetPaths(&jni, NULL, jni->NewStringUTF(&jni, "ux0:/data/hcr/base.apk"));
    Game_onControllerConnectionEvent(&jni, NULL, JNI_TRUE, 5);
    Game_nativeInit(&jni, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);
    Game_nativeResize(&jni, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (1)
    {
        controls_poll();

        Game_nativeRender(&jni);

        gl_swap();
    }

    audio_destroy();
    prefs_destroy();

    return sceKernelExitDeleteThread(0);
}
