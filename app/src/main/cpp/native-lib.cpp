#include <jni.h>
#include <string>
#include "IDemux.h"
#include "FFDemux.h"
#include "PlayerLog.h"

extern "C"
{

}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    IDemux *de = new FFDemux();
    de->open("/sdcard/Download/v1080.mp4");
//    for(;;){
//        PlayerData data = de->Read();
//        LOGD("Read data size is %d",data.size);
//        if (data.size==0)break;
//    }
    de->startThread();
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}