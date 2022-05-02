//
// Created by chen jy on 2022/5/1.
//

#ifndef CJYPLAYER_FFRESAMPLE_H
#define CJYPLAYER_FFRESAMPLE_H

#include "IResample.h"

struct SwrContext;

class FFResample : public IResample {
public:
    virtual bool
    open(PlayerParameter inParameter, PlayerParameter outParameter = PlayerParameter());

    //重新采样
    virtual PlayerData resample(PlayerData inData);

protected:
    SwrContext *actx = 0;
};


#endif //CJYPLAYER_FFRESAMPLE_H
