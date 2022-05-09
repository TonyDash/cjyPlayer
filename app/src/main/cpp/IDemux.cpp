//
// Created by chen jy on 2022/4/26.
//

#include "IDemux.h"
#include "PlayerLog.h"

void IDemux::main() {
    while (!isExit) {

        if(IsPause())
        {
            playerSleep(2);
            continue;
        }

        PlayerData data = Read();
        if (data.size>0){
            notify(data);
        } else{
            playerSleep(2);
        }
//        LOGD("Read data size is %d", data.size);
        //if (data.size <= 0)break;
    }
    LOGD("exit main");
}