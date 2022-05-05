//
// Created by chen jy on 2022/5/4.
//

#ifndef CJYPLAYER_IPLAYER_H
#define CJYPLAYER_IPLAYER_H

#include "PlayerThread.h"

//这里不直接include，是为了不需要提供所有的头文件给用户
//利用class声明，就只需要提供一个IPlayer的头文件
//增加保密性
class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;

class IPlayer : public PlayerThread {
public:
    static IPlayer *get(unsigned char index = 0);

    virtual bool open(const char *path);

    virtual bool startThread();
    //视频是否硬解码
    bool isHardDecode = true;

    IDemux *demux = 0;
    IDecode *vDecode = 0;
    IDecode *aDecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;
protected:
    IPlayer(){};
};


#endif //CJYPLAYER_IPLAYER_H
