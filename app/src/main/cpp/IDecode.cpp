//
// Created by chen jy on 2022/4/28.
//

#include "IDecode.h"
#include "PlayerLog.h"

void IDecode::clear() {
    packetMutex.lock();
    while (!packets.empty()){
        packets.front().drop();
        packets.pop_front();
    }
    pts = 0;
    synPts =0;
    packetMutex.unlock();
}

void IDecode::main() {
    while (!isExit){

        if(IsPause())
        {
            playerSleep(2);
            continue;
        }

        packetMutex.lock();
        //判断音视频同步
        //不是音频，同时音频同步基数大于0
        if (!isAudio&&synPts>0){
            //音频时间小于视频时间
            //音频播放慢于视频
            if (synPts<pts){
                packetMutex.unlock();
                playerSleep(1);
                continue;
            }
        }

        if (packets.empty()){
            packetMutex.unlock();
            //加sleep，为了避免当队列为空时，循环把CPU耗尽。
            //sleep会释放CPU
            playerSleep(1);
            continue;
        }
        //取出packet
        //消费者：从缓冲队列中消耗待处理事件
        PlayerData packs = packets.front();
        packets.pop_front();//消耗队列头部事件
        //发送数据到解码线程
        //一个数据包，可能解码多个结果
        //例如：音频有多种声道
        if (this->sendPacket(packs)){
            while (!isExit){
                //获取解码数据
                PlayerData frame = recvFrame();
                if (!frame.data){
                    break;
                }
                pts = frame.pts;
//                LOGD("recvFrame %d",frame.size);
                //发送数据到所有观察者
                this->notify(frame);
            }
        }
        packs.drop();
        packetMutex.unlock();
    }
}

void IDecode::update(PlayerData data) {
    if (data.isAudio != isAudio){
        return;
    }
    while (!isExit){
        packetMutex.lock();
        if (packets.size()<maxListSize){
            //生产者：往缓冲队列里添加待处理事件
            packets.push_back(data);
            packetMutex.unlock();
            break;
        }
        packetMutex.unlock();
        playerSleep(1);
    }



}