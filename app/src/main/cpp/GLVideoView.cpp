//
// Created by chen jy on 2022/4/30.
//

#include "GLVideoView.h"
#include "PlayerTexture.h"

void GLVideoView::setRender(void *win) {
    view = win;
}

void GLVideoView::close() {
    mutex.lock();
    if (txt) {
        //谁创建谁清理
        //txt是由PlayerTexture::create()创建的
        //所以不能直接delete，避免以后由于PlayerTexture的重构导致bug
        txt->drop();
        txt = 0;
    }

    mutex.unlock();
}

void GLVideoView::Render(PlayerData data) {
    if (!view)return;
    if (!txt) {
        txt = PlayerTexture::create();
        txt->init(view, static_cast<PlayerTextureType>(data.format));
    }
    txt->draw(data.datas, data.width, data.height);
}