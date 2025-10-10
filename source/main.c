#include "utils/init.h"
#include "utils/glutil.h"
#include "utils/logger.h"
#include "utils/utils.h"

#include <psp2/kernel/threadmgr.h>

#include <falso_jni/FalsoJNI.h>
#include <so_util/so_util.h>

#include <pthread.h>

int _newlib_heap_size_user = 256 * 1024 * 1024;

#ifdef USE_SCELIBC_IO
int sceLibcHeapSize = 4 * 1024 * 1024;
#endif

so_module so_mod;

void *pthread_main(void *arg)
{
    int (*JNI_OnLoad)(void *vm) = (void *)so_symbol(&so_mod, "JNI_OnLoad");
    int (*Game_setInAppItemPrice)(void *env, void *obj, jstring item, jstring price) = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_setInAppItemPrice");
    int (*Cocos2dx_nativeSetPaths)(void *env, void *obj, jstring apkFilePath) = (void *)so_symbol(&so_mod, "Java_org_cocos2dx_lib_Cocos2dxActivity_nativeSetPaths");
    int (*Game_isTestingMode)() = (void *)so_symbol(&so_mod, "Java_com_fingersoft_game_MainActivity_isTestingMode");

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

    l_debug("JNI_OnLoad");
    JNI_OnLoad(&jvm);

    l_debug("setInAppItemPrice");
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
    
    l_debug("isTestingMode");
    Game_isTestingMode();

    l_debug("nativeSetPaths");
    Cocos2dx_nativeSetPaths(&jni, NULL, jni->NewStringUTF(&jni, "ux0:/data/hcr/base.apk"));

    while (1)
    {
        // ... render call
        gl_swap();
    }

    return NULL;
}

int main()
{
    soloader_init_all();

    gl_init();

    pthread_t t2;
    pthread_attr_t attr2;
    pthread_attr_init(&attr2);
    pthread_attr_setstacksize(&attr2, 2 * 1024 * 1024);
    pthread_create(&t2, &attr2, pthread_main, NULL);

    return sceKernelExitDeleteThread(0);
}
