//
// Created by chen jy on 2022/4/26.
//

#ifndef CJYPLAYER_PLAYERDATA_H
#define CJYPLAYER_PLAYERDATA_H

//struct结构体
struct PlayerData {

    //清理函数
    void drop();
    unsigned char *data = 0;
    int size = 0;
    bool isAudio = false;
};


#endif //CJYPLAYER_PLAYERDATA_H
