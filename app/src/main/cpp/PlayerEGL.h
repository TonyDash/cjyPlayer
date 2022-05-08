//
// Created by chen jy on 2022/4/30.
//

#ifndef CJYPLAYER_PLAYEREGL_H
#define CJYPLAYER_PLAYEREGL_H

/**
 * OpenGL与窗口系统对应的适配层
 */
class PlayerEGL {
public:
    //定义为void，避免外部类型依赖
    virtual bool initEGL(void *win)=0;
    virtual void close()=0;
    virtual void draw() = 0;
    static PlayerEGL *get();//提供获取实例的方法,单例模式

protected:
    PlayerEGL(){}//protect的构造方法
};


#endif //CJYPLAYER_PLAYEREGL_H
