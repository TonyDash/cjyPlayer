//
// Created by chen jy on 2022/4/28.
//

#ifndef CJYPLAYER_FFDECODE_H
#define CJYPLAYER_FFDECODE_H

#include "PlayerParameter.h"
#include "IDecode.h"

struct AVCodecContext;

class FFDecode : public IDecode{
public:
    //打开解码器
    virtual bool open(PlayerParameter parameter);

protected:
    AVCodecContext *codec = 0;
};


#endif //CJYPLAYER_FFDECODE_H
