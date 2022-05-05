//
// Created by chen jy on 2022/4/27.
//

#ifndef CJYPLAYER_PLAYERTHREAD_H
#define CJYPLAYER_PLAYERTHREAD_H

void playerSleep(int mis);

class PlayerThread {
public:
    //启动线程
    virtual bool startThread();
    //停止线程,安全停止
    //由于线程状态不确定，停止线程是个比较危险的操作
    //但是由于性能和消耗问题，结束进程或者不需要继续执行时，又必须停止线程
    //所以需要安全地停止，例如增加一个标志位，通过对标志位的赋值来控制是否停止线程
    virtual void stopThread();
    //入口主函数
    virtual void main(){

    };
protected:
    bool isExit = false;
    bool isRunning = false;
private:
    virtual void threadMain();
};


#endif //CJYPLAYER_PLAYERTHREAD_H
