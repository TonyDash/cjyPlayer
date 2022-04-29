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

    if (!parameter.para) return false;
    AVCodecParameters *p = parameter.para;
    //查找解码器
    const AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (!cd) {
        LOGD("avcodec_find_decoder %d failed!", p->codec_id);
        return false;
    }

    //创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);//复制参数
    codec->thread_count = 8;//开启多线程解码

    //打开解码器
    //参数1：上下文  参数2：解码器   参数3：配置项
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);//最后一位不能用，是/0
        LOGE("%s", buf);
        return false;
    }
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO){
        isAudio = false;
    }else{
        //这里偷懒了，如果需要解码字幕，就会有问题
        //因为字幕不是视频流也不是音频流
        //严谨的话，需要再判断
        isAudio = true;
    }
    return true;
}

bool FFDecode::sendPacket(PlayerData data) {
    if (data.size <= 0 || !data.data)return false;
    if (!codec) {//存在多线程情况，需要单独判断
        return false;
    }
    int re = avcodec_send_packet(codec, reinterpret_cast<const AVPacket *>(data.data));
    if (re != 0) {
        return false;
    }

    return true;
}

PlayerData FFDecode::recvFrame() {
    if (!codec) {
        return {};
    }
    if (!frame) {
        frame = av_frame_alloc();//创建frame实例
    }
    //frame交给内部自己创建，可以重复利用
    int re = avcodec_receive_frame(codec, frame);
    if (re != 0) {
        return {};
    }
    PlayerData data;
    data.data = reinterpret_cast<unsigned char *>(frame);
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        //linesize:如果是平面模式，会有YUV三种路，每一路一行的大小
        //height：高度
        data.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
    }
    return data;
}