//
// Created by chen jy on 2022/4/30.
//

#ifndef CJYPLAYER_PLAYERSHADER_H
#define CJYPLAYER_PLAYERSHADER_H

#include <mutex>

enum PlayerShaderType{
    PLAYER_SHADER_YUV_420P = 0,// Y:4 U:1 V:1
    PLAYER_SHADER_YUV411P = 8,//
    PLAYER_SHADER_NV12 = 25, // Y:4 UV:1
    PLAYER_SHADER_NV21 = 26 //Y:4 VU:1
};


class PlayerShader {
public:
    virtual bool init(PlayerShaderType type = PLAYER_SHADER_YUV_420P);

    virtual void close();
    //获取材质并且映射到内存
    virtual void getTexture(unsigned int index, int width, int height, unsigned char *buf,bool isAlpha = false);

    virtual void draw();
protected:
    //需要清理，所以在这里定义为成员
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mutex;
};


#endif //CJYPLAYER_PLAYERSHADER_H
