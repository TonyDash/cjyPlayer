//
// Created by chen jy on 2022/4/30.
//

#ifndef CJYPLAYER_PLAYERTEXTURE_H
#define CJYPLAYER_PLAYERTEXTURE_H


class PlayerTexture {
public:
    static PlayerTexture *create();
    virtual bool init(void *win)=0;
};


#endif //CJYPLAYER_PLAYERTEXTURE_H
