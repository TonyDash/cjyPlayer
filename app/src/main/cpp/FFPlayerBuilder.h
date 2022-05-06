//
// Created by chen jy on 2022/5/6.
//

#ifndef CJYPLAYER_FFPLAYERBUILDER_H
#define CJYPLAYER_FFPLAYERBUILDER_H

#include "IPlayerBuilder.h"

class FFPlayerBuilder: public IPlayerBuilder {
public:
    static void initHard(void *vm);

    static FFPlayerBuilder *get(){
        static FFPlayerBuilder FFBuilder;
        return &FFBuilder;
    }

protected:

    FFPlayerBuilder(){};

    virtual IDemux *createDemux();

    virtual IDecode *createDecode();

    virtual IResample *createResample();

    virtual IVideoView *createVideoView();

    virtual IAudioPlay *createAudioPlay();

    virtual IPlayer *createPlayer(unsigned char index=0);
};


#endif //CJYPLAYER_FFPLAYERBUILDER_H
