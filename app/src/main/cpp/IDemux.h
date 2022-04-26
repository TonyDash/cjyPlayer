//
// Created by chen jy on 2022/4/26.
//

#ifndef CJYPLAYER_IDEMUX_H
#define CJYPLAYER_IDEMUX_H
#include "PlayerData.h"

//解封装接口
class IDemux {
public:
    //打开文件，或者流媒体、rtmp、rtsp、http
    virtual bool open(const char *url)=0;
    //读取一帧数据，数据有调用者自己清理
    virtual PlayerData Read()=0;

    //总时长(单位：毫秒)
    int totalMs = 0;
};


#endif //CJYPLAYER_IDEMUX_H
