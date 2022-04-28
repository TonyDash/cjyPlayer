//
// Created by chen jy on 2022/4/28.
//

#ifndef CJYPLAYER_IDECODE_H
#define CJYPLAYER_IDECODE_H

#include "PlayerParameter.h"
#include "IObserver.h"

//解码接口：支持硬解码
class IDecode : public IObserver{
public:
    //打开解码器
    virtual bool open(PlayerParameter parameter)=0;
};


#endif //CJYPLAYER_IDECODE_H
