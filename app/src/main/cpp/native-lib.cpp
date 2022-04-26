#include <jni.h>
#include <string>
#include "IDemux.h"
#include "FFDemux.h"

extern "C"
{

}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    IDemux *de = new FFDemux();
    de->open("/sdcard/Download/v1080.mp4");
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}