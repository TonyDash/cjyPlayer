//
// Created by chen jy on 2022/5/2.
//

#ifndef CJYPLAYER_IAUDIOPLAY_H
#define CJYPLAYER_IAUDIOPLAY_H


#include "IObserver.h"
#include "PlayerParameter.h"

class IAudioPlay: public IObserver {
public:
    virtual void Update(PlayerData data);
    virtual bool StartPlay(PlayerParameter out) = 0;
};


#endif //CJYPLAYER_IAUDIOPLAY_H
