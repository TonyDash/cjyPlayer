//
// Created by chen jy on 2022/4/26.
//

#ifndef CJYPLAYER_IDEMUX_H
#define CJYPLAYER_IDEMUX_H
#include "PlayerData.h"
#include "PlayerThread.h"
#include "IObserver.h"
#include "PlayerParameter.h"

//解封装接口
class IDemux : public IObserver {
public:
    //打开文件，或者流媒体、rtmp、rtsp、http
    virtual bool open(const char *url)=0;

    virtual void close()=0;
    virtual PlayerParameter getVPara()=0;

    //获取音频数据
    virtual PlayerParameter getAPara()=0;

    //读取一帧数据，数据有调用者自己清理
    virtual PlayerData Read()=0;

    //总时长(单位：毫秒)
    int totalMs = 0;
protected:
    virtual void main();
};


#endif //CJYPLAYER_IDEMUX_H
