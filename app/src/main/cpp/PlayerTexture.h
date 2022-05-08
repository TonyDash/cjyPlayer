//
// Created by chen jy on 2022/4/30.
//

#ifndef CJYPLAYER_PLAYERTEXTURE_H
#define CJYPLAYER_PLAYERTEXTURE_H

enum PlayerTextureType{
    PLAYER_TEXTURE_YUV_420P = 0,// Y:4 U:1 V:1
    PLAYER_TEXTURE_YUV411P = 8,// Y:4 U:1 V:1
    PLAYER_TEXTURE_NV12 = 25, // Y:4 UV:1
    PLAYER_TEXTURE_NV21 = 26 //Y:4 VU:1
};

class PlayerTexture {
public:
    static PlayerTexture *create();
    /***
     * 初始化
     * @param win 窗口引用
     * @param type 文件类型
     * @return
     */
    virtual bool init(void *win,PlayerTextureType type=PLAYER_TEXTURE_YUV_420P)=0;

    virtual void draw(unsigned char *data[],int width,int height)=0;
    virtual void drop()=0;
    //析构函数一定要virtual
    //因为析构函数后面需要添加自己的内容
    //
    virtual ~PlayerTexture(){};

protected:
    PlayerTexture(){};
};


#endif //CJYPLAYER_PLAYERTEXTURE_H
