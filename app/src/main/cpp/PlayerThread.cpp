//
// Created by chen jy on 2022/4/27.
//

#include "PlayerThread.h"
#include <thread>

//namespace不要放在头文件中，避免其他文件引用时，产生冲突
using namespace std;

void PlayerThread::startThread() {
    //函数的名称地址：&PlayerThread::threadMain
    //this:当前指针
    thread playerThread(&PlayerThread::threadMain, this);
    //放弃对该线程的控制
    //若不增加这句代码，当对象被清空时，相关的资源会出错
    playerThread.detach();
}

void PlayerThread::stopThread() {

}

/**
 * threadMain包裹main，是为了在同一个地方处理事情
 * 因为main函数会被子类重写，所以不适合在内部处理
 */
void PlayerThread::threadMain() {
    main();
}