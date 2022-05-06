//
// Created by chen jy on 2022/5/4.
//

#include "IPlayer.h"
#include "PlayerLog.h"
#include "PlayerParameter.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IAudioPlay.h"
#include "IVideoView.h"

IPlayer *IPlayer::get(unsigned char index) {
    //unsigned char最大值为255，所以IPlayer预设大小为256就足够用了
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::open(const char *path) {
    //解封装
    if (!demux || !demux->open(path)) {
        LOGE("demux->Open %s failed!", path);
        return false;
    }
    //解码 解码可能不需要，如果是解封之后就是原始数据
    if (!vDecode || !vDecode->open(demux->getVPara(), isHardDecode)) {
        LOGE("vdecode->Open %s failed!", path);
        //return false;
    }
    if (!aDecode || !aDecode->open(demux->getAPara())) {
        LOGE("adecode->Open %s failed!", path);
        //return false;
    }

    //重采样 有可能不需要，解码后或者解封后可能是直接能播放的数据
    //如果
    if (outPara.sample_rate <= 0) {
        outPara = demux->getAPara();
    }
    if (!resample || !resample->open(demux->getAPara(), outPara)) {
        LOGE("resample->Open %s failed!", path);
    }
    return true;
}

bool IPlayer::startThread() {
    if (!demux || !demux->startThread()) {
        LOGE("demux startThread failed");
        return false;
    }
    //视频可以快进快退，音频不可以，所以音频先于视频启动
    if (aDecode)aDecode->startThread();
    if (audioPlay)audioPlay->StartPlay(demux->getAPara());
    if (vDecode)vDecode->startThread();


    return true;
}

void IPlayer::initView(void *win) {
    if (videoView){
        videoView->setRender(win);
    }
}