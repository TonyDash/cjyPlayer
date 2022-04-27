//
// Created by chen jy on 2022/4/27.
//

#include "PlayerThread.h"
#include "PlayerLog.h"
#include <thread>

//namespace不要放在头文件中，避免其他文件引用时，产生冲突
using namespace std;

/**
 * 封装一个线程睡眠方法
 * @param mis 毫秒级别
 */
void playerSleep(int mis){
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

void PlayerThread::startThread() {
    isExit = false;
    //函数的名称地址：&PlayerThread::threadMain
    //this:当前指针
    thread playerThread(&PlayerThread::threadMain, this);
    //放弃对该线程的控制
    //若不增加这句代码，当对象被清空时，相关的资源会出错
    playerThread.detach();
}

void PlayerThread::stopThread() {
    isExit = true;
    for (int i = 0; i < 200; ++i) {
        if (!isRunning){
            LOGD("stopThread success!");
            return;
        }
        playerSleep(1);//等待200毫秒
    }
}

/**
 * threadMain包裹main，是为了在同一个地方处理事情
 * 因为main函数会被子类重写，所以不适合在内部处理
 */
void PlayerThread::threadMain() {
    isRunning = true;
    LOGI("threadMain start");
    main();
    LOGI("threadMain end");
    isRunning = false;
}