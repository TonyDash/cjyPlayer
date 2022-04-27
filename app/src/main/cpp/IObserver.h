//
// Created by chen jy on 2022/4/27.
//

#ifndef CJYPLAYER_IOBSERVER_H
#define CJYPLAYER_IOBSERVER_H

#include "PlayerData.h"
#include "PlayerThread.h"
#include <vector>
#include <mutex>//互斥变量

//观察者 和 主题
class IObserver : public PlayerThread{
public:
    //观察者接受数据
    virtual void update(PlayerData data) {}

    void addObs(IObserver *observer);

    //通知所有观察者
    void notify(PlayerData data);

protected:
    //需要考虑线程安全
    std::vector<IObserver *> vectorObs;
    std::mutex mux;
};


#endif //CJYPLAYER_IOBSERVER_H
