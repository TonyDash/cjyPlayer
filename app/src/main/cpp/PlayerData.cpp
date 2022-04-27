//
// Created by chen jy on 2022/4/26.
//

#include "PlayerData.h"

extern "C"{
#include <libavformat/avformat.h>
}

void PlayerData::drop() {
    if (!data) return;
    //释放空间
    //强转为指针的指针
    av_packet_free((AVPacket **)&data);
    data = 0;
    size = 0;
}
