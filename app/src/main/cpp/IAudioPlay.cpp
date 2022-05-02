//
// Created by chen jy on 2022/5/2.
//

#include "IAudioPlay.h"
#include "PlayerLog.h"

PlayerData IAudioPlay::getData() {
    PlayerData data;
    while (!isExit){
        framesMutex.lock();
        if (!frames.empty()){
            data = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            return data;
        }
        framesMutex.unlock();
        playerSleep(1);
    }


    return data;
}

void IAudioPlay::Update(PlayerData data) {
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
