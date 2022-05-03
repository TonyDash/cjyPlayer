//
// Created by chen jy on 2022/4/26.
//

#ifndef CJYPLAYER_PLAYERDATA_H
#define CJYPLAYER_PLAYERDATA_H

enum PlayerDataType
{
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

//struct结构体
struct PlayerData {

    //清理函数
    void drop();
    /**
     * 分配空间
     * @param size 分配大小
     * @param data 数据大小，默认0，就是不复制数据
     * @return
     */
    bool Alloc(int size,const char *data=0);
    int type = 0;
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};
    int size = 0;
    bool isAudio = false;
    int width = 0;
    int height = 0;
    //影片格式，决定是否使用硬解码
    int format = 0;
};


#endif //CJYPLAYER_PLAYERDATA_H
