//
// Created by chen jy on 2022/5/6.
//

#ifndef CJYPLAYER_IPLAYERPROXY_H
#define CJYPLAYER_IPLAYERPROXY_H

#include "IPlayer.h"
#include <mutex>

class IPlayerProxy : public IPlayer {
public:
    virtual bool open(const char *path);

    virtual void close();

    virtual bool startThread();

    virtual void initView(void *win);

    void init(void *vm=0);

    //获取当前的播放进度 0.0-1.0
    virtual double playPos();

    static IPlayerProxy *get() {
        static IPlayerProxy playerProxy;
        return &playerProxy;
    }

protected:
    IPlayerProxy(){};
    IPlayer *player = 0;
    std::mutex mutex;
};


#endif //CJYPLAYER_IPLAYERPROXY_H
