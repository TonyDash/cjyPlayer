//
// Created by chen jy on 2022/4/26.
//

#ifndef CJYPLAYER_FFDEMUX_H
#define CJYPLAYER_FFDEMUX_H

#include "IDemux.h"

class FFDemux: public IDemux {
public:
    //打开文件，或者流媒体、rtmp、rtsp、http
    virtual bool open(const char *url);
    //读取一帧数据，数据有调用者自己清理
    virtual PlayerData Read();
};


#endif //CJYPLAYER_FFDEMUX_H
