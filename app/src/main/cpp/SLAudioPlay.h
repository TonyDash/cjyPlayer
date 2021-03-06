//
// Created by chen jy on 2022/5/2.
//

#ifndef CJYPLAYER_SLAUDIOPLAY_H
#define CJYPLAYER_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay{
public:
    virtual void close();
    virtual bool StartPlay(PlayerParameter out);
    void PlayCall(void *bufq);
    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mutex;
};


#endif //CJYPLAYER_SLAUDIOPLAY_H
