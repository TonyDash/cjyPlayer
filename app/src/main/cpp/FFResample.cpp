//
// Created by chen jy on 2022/5/1.
//

#include "FFResample.h"
#include "PlayerLog.h"

extern "C"{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

bool
FFResample::open(PlayerParameter inParameter, PlayerParameter outParameter) {

    //音频重采样上下文初始化
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(outParameter.channels),
                              AV_SAMPLE_FMT_S16,outParameter.sample_rate,
                              av_get_default_channel_layout(inParameter.para->channels),
                              (AVSampleFormat)inParameter.para->format, inParameter.para->sample_rate, 0, 0);

    int re = swr_init(actx);
    if(re != 0)
    {
        LOGE("swr_init failed!");
        return false;
    }
    else
    {
        LOGI("swr_init success!");
    }
    outChannels = inParameter.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    return true;
}

PlayerData FFResample::resample(PlayerData inData) {

    if(inData.size<=0 || !inData.data) return {};
    if(!actx)
        return {};
    //XLOGE("indata size is %d",indata.size);
    AVFrame *frame = (AVFrame *)inData.data;

    //输出空间的分配
    PlayerData out;
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if(outsize <=0)return {};
    out.Alloc(outsize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    //音频重采样
    int len = swr_convert(actx,
                outArr,
                frame->nb_samples,
                (const uint8_t **)frame->data,
                frame->nb_samples);
    if(len<=0)
    {
        out.drop();
        return {};
    }
    out.pts = inData.pts;
    LOGE("swr_convert success = %d",len);
    return out;
}