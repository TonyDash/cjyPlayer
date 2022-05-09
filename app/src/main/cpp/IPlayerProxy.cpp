//
// Created by chen jy on 2022/5/6.
//

#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"

void IPlayerProxy::close() {
    mutex.lock();
    if (player)
        player->close();
    mutex.unlock();
}

double IPlayerProxy::playPos() {
    double pos = 0.0;
    mutex.lock();
    if (player){
        pos = player->playPos();
    }
    mutex.unlock();
    return pos;
}

bool IPlayerProxy::open(const char *path) {
    bool result = false;
    mutex.lock();
    if (player) {
        player->isHardDecode = isHardDecode;
        result = player->open(path);
    }
    mutex.unlock();
    return result;
}

bool IPlayerProxy::startThread() {
    bool result = false;
    mutex.lock();
    if (player)
        result = player->startThread();
    mutex.unlock();
    return result;
}

void IPlayerProxy::initView(void *win) {
    mutex.lock();
    if (player)
        player->initView(win);
    mutex.unlock();
}

void IPlayerProxy::init(void *vm) {
    mutex.lock();
    if (vm) {
        FFPlayerBuilder::initHard(vm);
    }
    if (!player)
        player = FFPlayerBuilder::get()->builderPlayer();
    mutex.unlock();
}