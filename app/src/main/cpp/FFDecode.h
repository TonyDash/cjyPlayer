//
// Created by chen jy on 2022/4/28.
//

#ifndef CJYPLAYER_FFDECODE_H
#define CJYPLAYER_FFDECODE_H

#include "PlayerParameter.h"
#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode : public IDecode{
public:
    static void initHard(void *vm);
    //打开解码器
    virtual bool open(PlayerParameter parameter,bool isHard = false);

    virtual void close();
    virtual void Clear();

    bool sendPacket(PlayerData data) override;

    PlayerData recvFrame() override;

protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
    std::mutex mutex;
};


#endif //CJYPLAYER_FFDECODE_H
