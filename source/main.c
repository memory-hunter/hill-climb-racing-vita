#include "utils/init.h"
#include "utils/glutil.h"
#include "utils/logger.h"
#include "utils/utils.h"

#include <psp2/kernel/threadmgr.h>

#include <falso_jni/FalsoJNI.h>
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

static void (*Cocos2dx_nativeTouchesBegin)(JNIEnv *jni, jobject thiz, jint id, jfloat x, jfloat y) = NULL;
static void (*Cocos2dx_nativeTouchesMove)(JNIEnv *jni, jobject thiz, jint *ids, jfloat *xs, jfloat *ys) = NULL;
static void (*Cocos2dx_nativeTouchesEnd)(JNIEnv *jni, jobject thiz, jint id, jfloat x, jfloat y) = NULL;
static int (*Cocos2dx_nativeKeyDown)(JNIEnv *jni, jobject thiz, jint keyCode) = NULL;

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
    switch (action)
    {
    case CONTROLS_ACTION_DOWN:
        Cocos2dx_nativeTouchesBegin(&jni, NULL, id, x, y);
        break;
    case CONTROLS_ACTION_MOVE:
    {
        Cocos2dx_nativeTouchesMove(&jni, NULL, &id, &x, &y);
        break;
    }
    break;
    case CONTROLS_ACTION_UP:
        Cocos2dx_nativeTouchesEnd(&jni, NULL, id, x, y);
        break;
    }
}

void controls_handler_analog(ControlsStickId which, float x, float y, ControlsAction action)
{
}

int main()
{
    soloader_init_all();

    gl_init();

    controls_init();

    int (*JNI_OnLoad)(void *vm) = (void *)so_symbol(&so_mod, "JNI_OnLoad");
    int (*Cocos2dx_nativeSetPaths)(void *env, void *obj, jstring apkFilePath) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxActivity_nativeSetPaths");
    int (*Game_isTestingMode)() = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_isTestingMode");
    int (*Game_nativeInit)(void *env, void *obj, jint screen_width, jint screen_height) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit");
    int (*Game_nativeResize)(void *env, void *obj, jint screen_width, jint screen_height) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeResize");
    int (*Game_nativeRender)(void *env) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender");

    Cocos2dx_nativeTouchesBegin = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin");
    Cocos2dx_nativeTouchesMove = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove");
    Cocos2dx_nativeTouchesEnd = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd");
    Cocos2dx_nativeKeyDown = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown");

    JNI_OnLoad(&jvm);
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

/*
Java_com_fingersoft_game_MainActivity_onControllerKeyEvent
Java_com_fingersoft_game_MainActivity_onControllerConnectionEvent
Java_com_fingersoft_utils_CloudHelper_onSignInCompleted
Java_com_fingersoft_utils_CloudHelper_onSyncDeviceLimitReached
Java_com_fingersoft_utils_CloudHelper_onSyncDataLoaded
Java_com_fingersoft_utils_CloudHelper_onAchievementProgressLoaded
Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeInitBitmapDC
Java_org_cocos2dx_lib_Cocos2dxAccelerometer_onSensorChanged
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeResize
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInsertText
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeDeleteBackward
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeGetContentText
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeOnPause
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeOnResume
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesCancel
Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown
*/