//
// Created by chen jy on 2022/4/26.
//

#ifndef CJYPLAYER_FFDEMUX_H
#define CJYPLAYER_FFDEMUX_H

#include "IDemux.h"
struct AVFormatContext;

class FFDemux: public IDemux {
public:
    //打开文件，或者流媒体、rtmp、rtsp、http
    virtual bool open(const char *url);

    virtual PlayerParameter getVPara();

    virtual PlayerParameter getAPara();

    //读取一帧数据，数据有调用者自己清理
    virtual PlayerData Read();

    FFDemux();//空的构造函数，ic才会初始化，如果不是空参构造函数，ic不会初始化。

private:
    AVFormatContext *ic = 0;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //CJYPLAYER_FFDEMUX_H
