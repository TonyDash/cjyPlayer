//
// Created by chen jy on 2022/4/27.
//

#include "IObserver.h"

void IObserver::addObs(IObserver *observer) {
    if (!observer)return;
    mux.lock();
    vectorObs.push_back(observer);
    mux.unlock();
}

void IObserver::notify(PlayerData data) {
    mux.lock();
    //size()方法每次都会重新统计，所以当数量级大的时候，不要在循环体内执行size()
    //外部确定值的大小
    unsigned int vectorSize = vectorObs.size();
    for (int i = 0; i < vectorSize; ++i) {
        vectorObs[i]->update(data);
    }

    mux.unlock();
}