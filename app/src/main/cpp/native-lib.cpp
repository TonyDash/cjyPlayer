#include <jni.h>
#include <string>
#include "IDemux.h"
#include "FFDemux.h"
#include "PlayerLog.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "PlayerEGL.h"
#include "PlayerShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "IResample.h"
#include "FFResample.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"
#include <android/native_window_jni.h>

extern "C"
{

}

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){
    FFDecode::initHard(vm);
    return JNI_VERSION_1_4;
}

IVideoView *view = NULL;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    IDemux *de = new FFDemux();
    de->open("/sdcard/Download/1080.mp4");
    IDecode *vdecode = new FFDecode();
    IDecode *adecode = new FFDecode();
    vdecode->open(de->getVPara(), true);
    adecode->open(de->getAPara());
    de->addObs(vdecode);
    de->addObs(adecode);

    view = new GLVideoView();
    vdecode->addObs(view);

    IResample *resample = new FFResample();
    PlayerParameter outParameter= de->getAPara();

    //这里增加音频采样会崩溃，暂时注释
//    resample->open(de->getAPara(),outParameter);
//    adecode->addObs(resample);

    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->StartPlay(outParameter);
    resample->addObs(audioPlay);

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
    view->setRender(window);

    //关联EGL
//    PlayerEGL::get()->initEGL(window);
//    PlayerShader shader;//shader不做单例，因为shader会有多个，有需求会有多路视频
//    shader.init();
}