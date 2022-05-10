//
// Created by chen jy on 2022/4/26.
//

#include "FFDemux.h"
#include "PlayerLog.h"

extern "C"
{
#include <libavformat/avformat.h>
}

//分数转为浮点数
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num / (double) r.den;
}


//seek 位置 pos 0.0~1.0
bool FFDemux::Seek(double pos)
{
    if(pos<0 || pos > 1)
    {
        LOGE("Seek value must 0.0~1.0");
        return false;
    }
    bool re = false;
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return false;
    }
    //清理读取的缓冲
    avformat_flush(ic);
    long long seekPts = 0;
    seekPts = ic->streams[videoStream]->duration*pos;

    //往后跳转到关键帧
    re = av_seek_frame(ic,videoStream,seekPts,AVSEEK_FLAG_FRAME|AVSEEK_FLAG_BACKWARD);
    mux.unlock();
    return re;
}

void FFDemux::close() {
    mutex.lock();
    if (ic)
        //这里不需要手动置NULL，avformat_close_input内部已经做了
        avformat_close_input(&ic);
    mutex.unlock();
}

//打开文件，或者流媒体、rtmp、rtsp、http
bool FFDemux::open(const char *url) {
    LOGD("open file %s begin", url);
    //open前先执行close
    close();
    mutex.lock();
    //后两个参数是什么用处
    int respone = avformat_open_input(&ic, url, 0, 0);
    if (respone != 0) {
        mutex.unlock();
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
        mutex.unlock();
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
    //一定要在getVPara和getAPara之前解锁
    //getVPara和getAPara里有自己的锁，如果这里不释放，会造成死锁
    mutex.unlock();
    LOGI("total ms = %d ", totalMs);
    getVPara();
    getAPara();
    return true;
}

PlayerParameter FFDemux::getVPara() {
    mutex.lock();
    if (!ic) {
        mutex.unlock();
        LOGD("ic is NULL !");
        return {};
    }
    //获取视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        mutex.unlock();
        LOGD("av_find_best_stream failed!");
        return {};
    }
    videoStream = re;
    PlayerParameter parameter;
    parameter.para = ic->streams[re]->codecpar;
    mutex.unlock();
    return parameter;
}

PlayerParameter FFDemux::getAPara() {
    mutex.lock();
    if (!ic) {
        mutex.unlock();
        LOGD("ic is NULL !");
        return {};
    }
    //获取音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        mutex.unlock();
        LOGD("av_find_best_stream failed!");
        return {};
    }
    audioStream = re;
    PlayerParameter parameter;
    parameter.para = ic->streams[re]->codecpar;
    parameter.channels = ic->streams[re]->codecpar->channels;
    parameter.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mutex.unlock();
    return parameter;
}

//读取一帧数据，数据有调用者自己清理
PlayerData FFDemux::Read() {
    mutex.lock();
    if (!ic) {
        mutex.unlock();
        return {};
    }
    PlayerData data;
    //av_packet_alloc:创建并且初始化
    AVPacket *packet = av_packet_alloc();
    //读取帧数据
    int re = av_read_frame(ic, packet);
    if (re != 0) {
        mutex.unlock();
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
        mutex.unlock();
        av_packet_free(&packet);
        return {};
    }
    //转换pts
    packet->pts = packet->pts * (1000 * r2d(ic->streams[packet->stream_index]->time_base));
    packet->dts = packet->dts * (1000 * r2d(ic->streams[packet->stream_index]->time_base));
    data.pts = (int) packet->pts;
//    LOGD("demux pts %d", data.pts);
    mutex.unlock();
    return data;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;

        //注册所有封装器
        av_register_all();

        //注册所有的解码器
        avcodec_register_all();

        //初始化网络
        avformat_network_init();
        LOGI("init ffmpeg network!");
    }
}
