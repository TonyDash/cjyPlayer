#include <jni.h>
#include <string>
#include "IDemux.h"
#include "FFDemux.h"
#include "PlayerLog.h"

extern "C"
{

}

class TestObserver: public IObserver{
public:
    void update(PlayerData data) override{
        LOGD("TestObserver update data size %d",data.size);
    }
};

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cjy_cjyplayer_activity_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    TestObserver *testObserver =new TestObserver();
    IDemux *de = new FFDemux();
    de->addObs(testObserver);
    de->open("/sdcard/Download/v1080.mp4");
//    for(;;){
//        PlayerData data = de->Read();
//        LOGD("Read data size is %d",data.size);
//        if (data.size==0)break;
//    }
    de->startThread();
    playerSleep(3000);//睡眠3秒
    de->stopThread();
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}