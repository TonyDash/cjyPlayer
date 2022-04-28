//
// Created by chen jy on 2022/4/28.
//

#include "FFDecode.h"
#include "PlayerLog.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

bool FFDecode::open(PlayerParameter parameter) {

    if(!parameter.para) return false;
    AVCodecParameters *p = parameter.para;
    //查找解码器
    const AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (!cd){
        LOGD("avcodec_find_decoder %d failed!", p->codec_id);
        return false;
    }

    //创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec,p);//复制参数

    //打开解码器
    //参数1：上下文  参数2：解码器   参数3：配置项
    int re = avcodec_open2(codec,0,0);
    if (re!=0){
        char buf[1024] = {0};
        av_strerror(re,buf, sizeof(buf)-1);//最后一位不能用，是/0
        LOGE("%s",buf);
        return false;
    }
    return true;
}