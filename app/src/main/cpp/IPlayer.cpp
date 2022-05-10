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

void IPlayer::close() {
    mutex.lock();
    //先关闭主体线程，再清理观察者
    //清理同步线程
    PlayerThread::stopThread();
    //清理解封装
    if (demux)
    demux->stopThread();
    //清理解码
    if (vDecode)
    vDecode->stopThread();
    if (aDecode)
    aDecode->stopThread();
    if(audioPlay)
        audioPlay->stopThread();
    //清理缓冲队列
    if (vDecode)
    vDecode->clear();
    if (aDecode)
    aDecode->clear();
    if (audioPlay)
    audioPlay->clear();
    //清理资源
    if (audioPlay)
    audioPlay->close();
    if (videoView)
    videoView->close();
    if (vDecode)
    vDecode->close();
    if (aDecode)
    aDecode->close();
    if (demux)
    demux->close();
    mutex.unlock();
}

double IPlayer::playPos() {
    double pos = 0.0;
    mutex.lock();
    int total = 0;
    if (demux){
        total = demux->totalMs;
    }
    if (total>0){
        if (vDecode){
            pos = (double)vDecode->pts/(double)total;
        }
    }
    mutex.unlock();
//    LOGD("pos is %f",pos);
    return pos;
}
void IPlayer::SetPause(bool isP)
{
    mutex.lock();
    PlayerThread::SetPause(isP);
    if(demux)
        demux->SetPause(isP);
    if(vDecode)
        vDecode->SetPause(isP);
    if(aDecode)
        aDecode->SetPause(isP);
    if(audioPlay)
        audioPlay->SetPause(isP);
    mutex.unlock();
}

bool IPlayer::Seek(double pos)
{
    bool re = false;
    if(!demux) return false;

    //暂停所有线程
    SetPause(true);
    mutex.lock();
    //清理缓冲
    //2 清理缓冲队列
    if(vDecode)
        vDecode->clear(); //清理缓冲队列，清理ffmpeg的缓冲
    if(aDecode)
        aDecode->clear();
    if(audioPlay)
        audioPlay->clear();


    re = demux->Seek(pos); //seek跳转到关键帧
    if(!vDecode)
    {
        mutex.unlock();
        SetPause(false);
        return re;
    }
    //解码到实际需要显示的帧
    int seekPts = pos*demux->totalMs;
    while(!isExit)
    {
        PlayerData pkt = demux->Read();
        if(pkt.size<=0)break;
        if(pkt.isAudio)
        {
            if(pkt.pts < seekPts)
            {
                pkt.drop();
                continue;
            }
            //写入缓冲队列
            demux->notify(pkt);
            continue;
        }

        //解码需要显示的帧之前的数据
        vDecode->sendPacket(pkt);
        pkt.drop();
        PlayerData data = vDecode->recvFrame();
        if(data.size <=0)
        {
            continue;
        }
        if(data.pts >= seekPts)
        {
            //vdecode->Notify(data);
            break;
        }
    }
    mutex.unlock();

    SetPause(false);
    return re;
}

bool IPlayer::open(const char *path) {
    close();
    mutex.lock();
    //解封装
    if (!demux || !demux->open(path)) {
        mutex.unlock();
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
//    if (outPara.sample_rate <= 0) {
        outPara = demux->getAPara();
//    }
    if (!resample || !resample->open(demux->getAPara(), outPara)) {
        LOGE("resample->Open %s failed!", path);
    }
    mutex.unlock();
    return true;
}

bool IPlayer::startThread() {
    mutex.lock();
    if (vDecode)vDecode->startThread();
    if (!demux || !demux->startThread()) {
        mutex.unlock();
        LOGE("demux startThread failed");
        return false;
    }
    //视频可以快进快退，音频不可以，所以音频先于视频启动
    if (aDecode)aDecode->startThread();
    if (audioPlay)audioPlay->StartPlay(demux->getAPara());
    PlayerThread::startThread();
    mutex.unlock();
    return true;
}

void IPlayer::initView(void *win) {
    if (videoView) {
        videoView->close();
        videoView->setRender(win);
    }
}

void IPlayer::main() {
    while (!isExit) {
        mutex.lock();
        //内存被清理时，audioPlay和vDecode指针不一定存在
        //所以需要判断，避免崩溃
        if (!audioPlay ||!vDecode){
            mutex.unlock();
            playerSleep(2);
            continue;
        }
        //同步
        //获取音频的pts，通知视频
        //时间基数不统一
        int aPts = audioPlay->pts;
//        LOGD("apts = %d",aPts);
        vDecode->synPts = aPts;
        mutex.unlock();
        playerSleep(2);
    }

}