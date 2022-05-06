//
// Created by chen jy on 2022/5/6.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"


IDemux *FFPlayerBuilder::createDemux() {
    IDemux *iDemux = new FFDemux();
    return iDemux;
}

IDecode *FFPlayerBuilder::createDecode() {
    IDecode *iDecode = new FFDecode();
    return iDecode;
}

IResample *FFPlayerBuilder::createResample() {
    IResample *iResample = new FFResample();
    return iResample;
}

IVideoView *FFPlayerBuilder::createVideoView() {
    IVideoView *videoView = new GLVideoView();
    return videoView;
}

IAudioPlay *FFPlayerBuilder::createAudioPlay() {
    IAudioPlay *audioPlay = new SLAudioPlay();
    return audioPlay;
}

IPlayer *FFPlayerBuilder::createPlayer(unsigned char index) {
    return IPlayer::get(index);
}

void FFPlayerBuilder::initHard(void *vm) {
    FFDecode::initHard(vm);
}