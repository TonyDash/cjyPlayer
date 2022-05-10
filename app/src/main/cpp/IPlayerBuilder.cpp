//
// Created by chen jy on 2022/5/6.
//

#include "IPlayerBuilder.h"
#include "IVideoView.h"
#include "IResample.h"
#include "IAudioPlay.h"
#include "IDemux.h"
#include "IDecode.h"

IPlayer * IPlayerBuilder::builderPlayer(unsigned char index) {
    IPlayer *player = createPlayer(index);

    //解封装
    IDemux *de = createDemux();
    //视频解码
    IDecode *vdecode = createDecode();
    //音频解码
    IDecode *adecode = createDecode();

    //解码器观察封装
    de->addObs(vdecode);
    de->addObs(adecode);

    //显示观察解码器
    IVideoView *view = createVideoView();
    vdecode->addObs(view);

    //重采样观察音频解码器
    IResample *resample = createResample();
    adecode->addObs(resample);

    //音频播放观察重采样
    IAudioPlay *audioPlay = createAudioPlay();
    resample->addObs(audioPlay);

    player->demux = de;
    player->aDecode = adecode;
    player->vDecode = vdecode;
    player->videoView = view;
    player->resample = resample;
    player->audioPlay = audioPlay;
    return player;
}
