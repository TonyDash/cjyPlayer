//
// Created by chen jy on 2022/4/26.
//

#include "IDemux.h"
#include "PlayerLog.h"

void IDemux::main() {
    for(;;){
        PlayerData data = Read();
        LOGD("Read data size is %d",data.size);
        if (data.size<=0)break;
    }
}