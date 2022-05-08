//
// Created by chen jy on 2022/4/30.
//

#ifndef CJYPLAYER_GLVIDEOVIEW_H
#define CJYPLAYER_GLVIDEOVIEW_H


#include "PlayerData.h"
#include "IVideoView.h"

class PlayerTexture;

class GLVideoView :public IVideoView{

public:
    virtual void setRender(void *win);
    virtual void Render(PlayerData data);
    virtual void close();

protected:
    void *view = 0;
    PlayerTexture *txt = 0;
    std::mutex mutex;
};


#endif //CJYPLAYER_GLVIDEOVIEW_H
