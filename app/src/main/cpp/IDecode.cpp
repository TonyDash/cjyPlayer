//
// Created by chen jy on 2022/4/28.
//

#include "IDecode.h"
#include "PlayerLog.h"

void IDecode::main() {
    while (!isExit){
        packetMutex.lock();
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
//                LOGD("recvFrame %d",frame.size);
                //发送数据到所有观察者
                this->notify(packs);
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