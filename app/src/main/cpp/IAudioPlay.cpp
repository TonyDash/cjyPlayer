//
// Created by chen jy on 2022/5/2.
//

#include "IAudioPlay.h"
#include "PlayerLog.h"

PlayerData IAudioPlay::getData() {
    PlayerData data;

    isRunning = true;
    while (!isExit){

        if(IsPause())
        {
            playerSleep(2);
            continue;
        }
        framesMutex.lock();
        if (!frames.empty()){
            //有数据返回
            data = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = data.pts;
            return data;
        }
        framesMutex.unlock();
        playerSleep(1);
    }
    isRunning = false;
    //未获取数据
    return data;
}

void IAudioPlay::update(PlayerData data) {
    LOGD("iAudioPlay::Update %d",data.size);
    //压入缓冲队列
    if (data.size <= 0 || !data.data)return;
    while (!isExit) {
        framesMutex.lock();
        if (frames.size() > maxFrame) {
            framesMutex.unlock();
            playerSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
    }

}

void IAudioPlay::clear() {
    framesMutex.lock();
    while (!frames.empty()){
        frames.front().drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}
