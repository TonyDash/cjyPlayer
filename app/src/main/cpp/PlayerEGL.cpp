//
// Created by chen jy on 2022/4/30.
//

#include "PlayerEGL.h"
class CPlayerEGL: public PlayerEGL{
public:
    bool initEGL(void *win) override{
        return true;
    }
};

//保证单例
PlayerEGL * PlayerEGL::get() {
    static CPlayerEGL egl;
    return &egl;
}

