//
// Created by chen jy on 2022/4/30.
//

#include "GLVideoView.h"
#include "PlayerTexture.h"

void GLVideoView::setRender(void *win) {
    view = win;
}

void GLVideoView::Render(PlayerData data) {
    if(!view)return;
    if(!txt){
        txt = PlayerTexture::create();
        txt->init(view, static_cast<PlayerTextureType>(data.format));
    }
    txt->draw(data.datas,data.width,data.height);
}