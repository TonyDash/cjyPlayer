#include <jni.h>
#include <string>
#include "IDemux.h"
#include "FFDemux.h"
#include "PlayerLog.h"
#include "IDecode.h"
#include "FFDecode.h"

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