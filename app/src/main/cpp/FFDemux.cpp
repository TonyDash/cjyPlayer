//
// Created by chen jy on 2022/4/26.
//

#include "FFDemux.h"
#include "PlayerLog.h"

//打开文件，或者流媒体、rtmp、rtsp、http
bool FFDemux::open(const char *url) {
    LOGD("open file %s begin", url);
    return true;
}

//读取一帧数据，数据有调用者自己清理
PlayerData FFDemux::Read() {
    PlayerData data;
    return data;
}
