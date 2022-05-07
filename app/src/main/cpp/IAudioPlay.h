//
// Created by chen jy on 2022/5/2.
//

#ifndef CJYPLAYER_IAUDIOPLAY_H
#define CJYPLAYER_IAUDIOPLAY_H


#include "IObserver.h"
#include "PlayerParameter.h"
#include <list>

class IAudioPlay: public IObserver {
public:
    //缓冲区如果打到最大值，阻塞
    virtual void Update(PlayerData data);
    //获取缓冲队列中的数据，若没有数据，则阻塞
    virtual PlayerData getData();
    virtual bool StartPlay(PlayerParameter out) = 0;
    //最大缓冲 100帧 25秒一帧计算，就是缓冲能4秒
    int maxFrame = 100;
    int pts = 0;
protected:
    std::list<PlayerData> frames;
    std::mutex framesMutex;
};


#endif //CJYPLAYER_IAUDIOPLAY_H
