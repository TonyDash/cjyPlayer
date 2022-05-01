//
// Created by chen jy on 2022/4/30.
//

#ifndef CJYPLAYER_IVIDEOVIEW_H
#define CJYPLAYER_IVIDEOVIEW_H


#include "PlayerData.h"
#include "IObserver.h"

class IVideoView: public IObserver {
public:
    virtual void setRender(void *win) =0;
    virtual void Render(PlayerData data)=0;
    virtual void update(PlayerData data);
};


#endif //CJYPLAYER_IVIDEOVIEW_H
