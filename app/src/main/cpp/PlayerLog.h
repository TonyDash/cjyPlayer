//
// Created by chen jy on 2022/4/26.
//
//宏定义全局函数:C++打印log到android-debug模式下帮助调试(勿删)
//调用方式：LOGD("test number=%d", number);

#ifndef CJYPLAYER_PLAYERLOG_H
#define CJYPLAYER_PLAYERLOG_H

#ifdef ANDROID
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"CJY_PLAYER",__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"CJY_PLAYER",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"CJY_PLAYER",__VA_ARGS__)
#else
#define LOGD(...) printf("CJY_PLAYER",__VA_ARGS__)
#define LOGI(...) printf("CJY_PLAYER",__VA_ARGS__)
#define LOGE(...) printf("CJY_PLAYER",__VA_ARGS__)
#endif


class PlayerLog {

};

#endif //CJYPLAYER_PLAYERLOG_H
