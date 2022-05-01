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
    unsigned char *datas[8] = {0};
    int size = 0;
    bool isAudio = false;
    int width = 0;
    int height = 0;
};


#endif //CJYPLAYER_PLAYERDATA_H
