#include <jni.h>
#include <string>
#include "IDemux.h"
#include "FFDemux.h"
#include "PlayerLog.h"
#include "IDecode.h"
#include "FFDecode.h"
#include <android/native_window_jni.h>

extern "C"
{

}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    IDemux *de = new FFDemux();
    de->open("/sdcard/Download/v1080.mp4");
    IDecode *vdecode = new FFDecode();
    IDecode *adecode = new FFDecode();
    vdecode->open(de->getVPara());
    adecode->open(de->getAPara());
    de->addObs(vdecode);
    de->addObs(adecode);

    de->startThread();
    vdecode->startThread();
    adecode->startThread();
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_cjy_cjyplayer_activity_CjyPlayer_initView(JNIEnv *env, jobject thiz,
                                                   jobject surface_view) {
    //创建窗口对象
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface_view);
    //关联EGL
}