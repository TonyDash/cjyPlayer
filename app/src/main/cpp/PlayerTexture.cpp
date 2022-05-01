//
// Created by chen jy on 2022/4/30.
//

#include "PlayerTexture.h"
#include "PlayerLog.h"
#include "PlayerEGL.h"
#include "PlayerShader.h"

class CPlayerTexture : public PlayerTexture{
public:
    PlayerShader playerShader;

    virtual bool init(void *win){
        if (!win){
            LOGD("CPlayerTexture init failed win is null");
            return false;
        }
        if (PlayerEGL::get()->initEGL(win)){
            return false;
        }
        playerShader.init();

        return true;
    }

    virtual void draw(unsigned char **data, int width, int height) override{
        //420:UV共用一个Y，所以大小只需要Y的一半就可以了
        playerShader.getTexture(0,width,height,data[0]);//Y
        playerShader.getTexture(0,width/2,height/2,data[1]);//U
        playerShader.getTexture(0,width/2,height/2,data[2]);//V

        //开始显示
        playerShader.draw();
        PlayerEGL::get()->draw();
    }
};

PlayerTexture * PlayerTexture::create() {
    return new CPlayerTexture();
}