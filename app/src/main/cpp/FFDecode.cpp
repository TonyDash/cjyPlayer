//
// Created by chen jy on 2022/4/28.
//

#include "FFDecode.h"
#include "PlayerLog.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}

void FFDecode::initHard(void *vm) {
    av_jni_set_java_vm(vm, 0);
}
void FFDecode::clear()
{
    IDecode::clear();
    mux.lock();
    if(codec)
        avcodec_flush_buffers(codec);
    mux.unlock();
}

void FFDecode::close() {
    IDecode::clear();
    mutex.lock();
    pts = 0;
    if (frame) {
        av_frame_free(&frame);
    }
    if (codec) {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mutex.unlock();
}

bool FFDecode::open(PlayerParameter parameter, bool isHard) {
    close();
    if (!parameter.para) return false;
    AVCodecParameters *p = parameter.para;
    //查找解码器
    const AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (isHard) {//硬解码
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if (!cd) {
        LOGD("avcodec_find_decoder %d failed!  isHard=%d", p->codec_id, isHard);
        return false;
    }
    LOGD("avcodec_find_decoder_by_name success %d", isHard);
    mutex.lock();
    //创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);//复制参数
    codec->thread_count = 8;//开启多线程解码

    //打开解码器
    //参数1：上下文  参数2：解码器   参数3：配置项
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0) {
        mutex.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);//最后一位不能用，是/0
        LOGE("%s", buf);
        return false;
    }
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
    } else {
        //这里偷懒了，如果需要解码字幕，就会有问题
        //因为字幕不是视频流也不是音频流
        //严谨的话，需要再判断
        this->isAudio = true;
    }
    mutex.unlock();
    return true;
}

bool FFDecode::sendPacket(PlayerData data) {
    if (data.size <= 0 || !data.data)return false;
    mutex.lock();
    if (!codec) {//存在多线程情况，需要单独判断
        mutex.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec, reinterpret_cast<const AVPacket *>(data.data));
    mutex.unlock();
    if (re != 0) {
        return false;
    }

    return true;
}

PlayerData FFDecode::recvFrame() {
    mutex.lock();
    if (!codec) {
        mutex.unlock();
        return {};
    }
    if (!frame) {
        frame = av_frame_alloc();//创建frame实例
    }
    //frame交给内部自己创建，可以重复利用
    int re = avcodec_receive_frame(codec, frame);
    if (re != 0) {
        mutex.unlock();
        return {};
    }
    PlayerData data;
    data.data = reinterpret_cast<unsigned char *>(frame);
    if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        //linesize:如果是平面模式，会有YUV三种路，每一路一行的大小
        //height：高度
        data.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        data.width = frame->width;
        data.height = frame->height;
    } else {
        //样本字节数 * 单通道样本数 * 通道数
//        data.size = av_get_bytes_per_sample(static_cast<AVSampleFormat>(frame->format)) *
//                    frame->nb_samples * 2;

        data.size = av_get_bytes_per_sample((AVSampleFormat) frame->format) * frame->nb_samples * 2;
    }
//    if (!isAudio)
//    LOGD("data format is %d",frame->format);
    data.format = frame->format;
    //复制数据
    memcpy(data.datas, frame->data, sizeof(data.datas));
    data.pts = frame->pts;
    pts = data.pts;
    mutex.unlock();
    return data;
}