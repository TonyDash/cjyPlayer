#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include "FFPlayerBuilder.h"

extern "C"
{

}
//IVideoView *view = NULL;
static IPlayer *player = NULL;

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
//    FFDecode::initHard(vm);

    FFPlayerBuilder::initHard(vm);
    player = FFPlayerBuilder::get()->builderPlayer();

    player->open("/sdcard/Download/1080.mp4");

    player->startThread();
//    de->startThread();
//    vdecode->startThread();
//    adecode->startThread();

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
//    view->setRender(window);
    if (player)
        player->initView(window);
    //关联EGL
//    PlayerEGL::get()->initEGL(window);
//    PlayerShader shader;//shader不做单例，因为shader会有多个，有需求会有多路视频
//    shader.init();
}