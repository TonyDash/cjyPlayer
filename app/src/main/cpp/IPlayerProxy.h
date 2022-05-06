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

    virtual bool startThread();

    virtual void initView(void *win);

    void init(void *vm=0);

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
