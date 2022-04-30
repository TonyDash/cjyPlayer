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
};

PlayerTexture * PlayerTexture::create() {
    return new CPlayerTexture();
}