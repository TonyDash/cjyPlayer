//
// Created by chen jy on 2022/4/28.
//

#ifndef CJYPLAYER_IDECODE_H
#define CJYPLAYER_IDECODE_H

#include "PlayerParameter.h"
#include "IObserver.h"
#include <list>

//解码接口：支持硬解码
class IDecode : public IObserver{
public:
    //打开解码器
    virtual bool open(PlayerParameter parameter,bool isHard = false)=0;

    virtual void close()=0;
    //future模型 发送数据到线程解码
    virtual bool sendPacket(PlayerData data)=0;

    //从线程中获取解码结果，再次调用会复用上次空间
    //线程不安全
    virtual PlayerData recvFrame() = 0;

    //由主体notify通知的数据  阻塞
    virtual void update(PlayerData data);

    bool isAudio = false;

    //队列的最大值
    //100帧画面：如果一秒是25帧，那缓存最大值就是缓存4秒
    int maxListSize = 100;


    //同步时间，再次打开文件要清理
    int synPts = 0;
    int pts = 0;

protected:
    virtual void main();
    //缓冲队列
    std::list<PlayerData> packets;
    //队列读取的互斥变量
    std::mutex packetMutex;
};


#endif //CJYPLAYER_IDECODE_H
