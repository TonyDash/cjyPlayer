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


bool PlayerData::Alloc(int size,const char *d)
{
    drop();//清理空间
    type = UCHAR_TYPE;
    if(size <=0)return false;
    this->data = new unsigned char[size];
    if(!this->data) return false;
    if(d)
    {
        memcpy(this->data,d,size);//复制数据
    }
    return true;
}
