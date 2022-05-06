//
// Created by chen jy on 2022/5/6.
//

#ifndef CJYPLAYER_IPLAYERBUILDER_H
#define CJYPLAYER_IPLAYERBUILDER_H

#include "IPlayer.h"

class IPlayerBuilder {
public:

    virtual IPlayer *builderPlayer(unsigned char index = 0);

protected:
    virtual IDemux *createDemux() = 0;

    virtual IDecode *createDecode() = 0;

    virtual IResample *createResample() = 0;

    virtual IVideoView *createVideoView() = 0;

    virtual IAudioPlay *createAudioPlay() = 0;

    virtual IPlayer *createPlayer(unsigned char index=0) = 0;
};


#endif //CJYPLAYER_IPLAYERBUILDER_H
