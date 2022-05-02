//
// Created by chen jy on 2022/4/26.
//

#include "FFDemux.h"
#include "PlayerLog.h"

extern "C"
{
#include <libavformat/avformat.h>
}

//打开文件，或者流媒体、rtmp、rtsp、http
bool FFDemux::open(const char *url) {
    LOGD("open file %s begin", url);
    //后两个参数是什么用处
    int respone = avformat_open_input(&ic, url, 0, 0);
    if (respone != 0) {
        char buf[1024] = {0};
        //这个函数的用法是什么
        av_strerror(respone, buf, sizeof(buf));
        LOGE("avformat_open_input %s failed!", url);
        return false;
    }
    LOGI("open %s success!", url);

    //读取文件信息
    //第二个参数是什么
    respone = avformat_find_stream_info(ic, 0);
    if (respone != 0) {
        char buf[1024] = {0};
        //这个函数的用法是什么
        av_strerror(respone, buf, sizeof(buf));
        LOGE("avformat_find_stream_info %s failed!", url);
        return false;
    }
    //AV_TIME_BASE:时间基数单位，代表的是1秒里的单位个数
    //这里我想打印毫秒值，所以用AV_TIME_BASE除以1000，AV_TIME_BASE默认是纳秒值
    //得到的就是毫秒值
    this->totalMs = ic->duration / (AV_TIME_BASE / 1000);
    LOGI("total ms = %d ", totalMs);
    getVPara();
    getAPara();
    return true;
}

PlayerParameter FFDemux::getVPara() {
    if (!ic) {
        LOGD("ic is NULL !");
        return {};
    }
    //获取视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        LOGD("av_find_best_stream failed!");
        return {};
    }
    videoStream = re;
    PlayerParameter parameter;
    parameter.para = ic->streams[re]->codecpar;
    parameter.channels = ic->streams[re]->codecpar->channels;
    parameter.sample_rate = ic->streams[0]->codecpar->sample_rate;
    return parameter;
}

PlayerParameter FFDemux::getAPara() {
    if (!ic) {
        LOGD("ic is NULL !");
        return {};
    }
    //获取音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        LOGD("av_find_best_stream failed!");
        return {};
    }
    audioStream = re;
    PlayerParameter parameter;
    parameter.para = ic->streams[re]->codecpar;
    return parameter;
}

//读取一帧数据，数据有调用者自己清理
PlayerData FFDemux::Read() {

    if (!ic)return {};
    PlayerData data;
    //av_packet_alloc:创建并且初始化
    AVPacket *packet = av_packet_alloc();
    //读取帧数据
    int re = av_read_frame(ic, packet);
    if (re != 0) {
        av_packet_free(&packet);//清空对象并且减少引用计数
        return {};
    }
//    LOGI("packet size is %d pts is %lld",packet->size,packet->pts);
    data.data = (unsigned char *) packet;
    data.size = packet->size;
    if (packet->stream_index == audioStream) {
        data.isAudio = true;
    } else if (packet->stream_index == videoStream) {
        data.isAudio = false;
    } else {
        //这里一定要销毁
        //av_packet_alloc会分配出一个对象
        //av_read_frame又会分配一个空间
        //如果不释放会发生内存泄漏
        av_packet_free(&packet);
        return {};
    }
    return data;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;
        //初始化网络
        avformat_network_init();
        LOGI("init ffmpeg network!");
    }
}
