//
// Created by chen jy on 2022/5/1.
//

#ifndef CJYPLAYER_IRESAMPLE_H
#define CJYPLAYER_IRESAMPLE_H


#include "IObserver.h"
#include "PlayerParameter.h"

class IResample :public IObserver {
public:
    virtual bool open(PlayerParameter inParameter, PlayerParameter outParameter=PlayerParameter()) = 0;
    virtual PlayerData resample(PlayerData inData) = 0;
    virtual void update(PlayerData data);
    int outChannels = 2;
    int outFormat = 1;
};


#endif //CJYPLAYER_IRESAMPLE_H
