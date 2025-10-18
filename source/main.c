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

int _newlib_heap_size_user = 256 * 1024 * 1024;

#ifdef USE_SCELIBC_IO
int sceLibcHeapSize = 4 * 1024 * 1024;
#endif

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

so_module so_mod;

void (*Cocos2dx_nativeTouchesBegin)(JNIEnv *jni, jobject thiz, jint id, jfloat x, jfloat y);
void (*Cocos2dx_nativeTouchesMove)(JNIEnv *jni, jobject thiz, jint *ids, jfloat *xs, jfloat *ys);
void (*Cocos2dx_nativeTouchesEnd)(JNIEnv *jni, jobject thiz, jint id, jfloat x, jfloat y);
int (*Cocos2dx_nativeKeyDown)(JNIEnv *jni, jobject thiz, jint keyCode);

JavaDynArray *touch_ids, *touch_xs, *touch_ys;

void controls_handler_key(int32_t keycode, ControlsAction action)
{
    switch (keycode)
    {
    case AKEYCODE_BUTTON_L1:
        controls_handler_touch(69, 156.0, 410.0, action);
        return;
    case AKEYCODE_BUTTON_R1:
        controls_handler_touch(420, 817.0, 410.0, action);
        return;
    }
    Cocos2dx_nativeKeyDown(&jni, NULL, keycode);
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
        Cocos2dx_nativeTouchesMove(&jni, NULL, touch_ids, touch_xs, touch_ys);
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

    gl_init();

    controls_init();

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

    Cocos2dx_nativeTouchesBegin = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin");
    Cocos2dx_nativeTouchesMove = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove");
    Cocos2dx_nativeTouchesEnd = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd");
    Cocos2dx_nativeKeyDown = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown");

    JNI_OnLoad(&jvm);

    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.adfree_150000coins"), jni->NewStringUTF(&jni, "1.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.adfree_300000coins"), jni->NewStringUTF(&jni, "2.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.adfree_750000coins"), jni->NewStringUTF(&jni, "4.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.adfree_2000000coins"), jni->NewStringUTF(&jni, "9.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.adfree_4000000coins"), jni->NewStringUTF(&jni, "16.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.adfree_8000000coins"), jni->NewStringUTF(&jni, "25.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.adfree_20000000coins"), jni->NewStringUTF(&jni, "49.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap2.adfree_300000coins"), jni->NewStringUTF(&jni, "1.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap2.adfree_600000coins"), jni->NewStringUTF(&jni, "2.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap2.adfree_1500000coins"), jni->NewStringUTF(&jni, "4.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap2.adfree_4000000coins"), jni->NewStringUTF(&jni, "9.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap2.adfree_8000000coins"), jni->NewStringUTF(&jni, "16.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap2.adfree_16000000coins"), jni->NewStringUTF(&jni, "25.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap2.adfree_40000000coins"), jni->NewStringUTF(&jni, "49.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap3.adfree_600000coins"), jni->NewStringUTF(&jni, "1.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap3.adfree_1200000coins"), jni->NewStringUTF(&jni, "2.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap3.adfree_3000000coins"), jni->NewStringUTF(&jni, "4.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap3.adfree_8000000coins"), jni->NewStringUTF(&jni, "9.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap3.adfree_16000000coins"), jni->NewStringUTF(&jni, "16.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap3.adfree_32000000coins"), jni->NewStringUTF(&jni, "25.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap3.adfree_80000000coins"), jni->NewStringUTF(&jni, "49.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap4.adfree_1200000coins"), jni->NewStringUTF(&jni, "1.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap4.adfree_2400000coins"), jni->NewStringUTF(&jni, "2.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap4.adfree_6000000coins"), jni->NewStringUTF(&jni, "4.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap4.adfree_16000000coins"), jni->NewStringUTF(&jni, "9.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap4.adfree_32000000coins"), jni->NewStringUTF(&jni, "16.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap4.adfree_64000000coins"), jni->NewStringUTF(&jni, "25.99"));
    Game_setInAppItemPrice(&jni, NULL, jni->NewStringUTF(&jni, "com.fingersoft.hillclimb.iap4.adfree_160000000coins"), jni->NewStringUTF(&jni, "49.99"));

    Game_isTestingMode();
    Cocos2dx_nativeSetPaths(&jni, NULL, jni->NewStringUTF(&jni, "ux0:/data/hcr/base.apk"));
    Game_nativeInit(&jni, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (1)
    {
        controls_poll();

        Game_nativeResize(&jni, NULL, SCREEN_WIDTH, SCREEN_HEIGHT);
        Game_nativeRender(&jni);

        gl_swap();
    }

    return sceKernelExitDeleteThread(0);
}
