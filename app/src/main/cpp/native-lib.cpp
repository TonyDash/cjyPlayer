#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "IPlayerProxy.h"

extern "C"
{

}

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
//    FFDecode::initHard(vm);
IPlayerProxy::get()->init(vm);
IPlayerProxy::get()->open("/sdcard/Download/1080.mp4");
IPlayerProxy::get()->startThread();
    return JNI_VERSION_1_4;
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_cjy_cjyplayer_activity_CjyPlayer_initView(JNIEnv *env, jobject thiz,
                                                   jobject surface_view) {
    //创建窗口对象
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface_view);
    IPlayerProxy::get()->initView(window);
}