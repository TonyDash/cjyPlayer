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
//    IPlayerProxy::get()->open("/sdcard/Download/v1080.mp4");
//    IPlayerProxy::get()->startThread();

    /**测试多次打开文件
     * 看是否上一次资源是否有被释放和其他bug
     * **/
//    IPlayerProxy::get()->open("/sdcard/Download/1080.mp4");
//    IPlayerProxy::get()->startThread();
    /****/

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
extern "C"
JNIEXPORT void JNICALL
Java_com_cjy_cjyplayer_activity_ChoiceFileActivity_open(JNIEnv *env, jobject thiz, jstring url_) {
    const char *url = env->GetStringUTFChars(url_,0);
    IPlayerProxy::get()->open(url);
    IPlayerProxy::get()->startThread();
    env->ReleaseStringUTFChars(url_,url);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_PlayPos(JNIEnv *env, jobject thiz) {
    return IPlayerProxy::get()->playPos();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_Seek(JNIEnv *env, jobject thiz, jdouble pos) {
    IPlayerProxy::get()->Seek(pos);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_cjy_cjyplayer_activity_CjyPlayer_PlayOrPause(JNIEnv *env, jobject thiz) {
    IPlayerProxy::get()->SetPause(!IPlayerProxy::get()->IsPause());
}