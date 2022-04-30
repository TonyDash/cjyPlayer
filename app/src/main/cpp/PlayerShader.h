//
// Created by chen jy on 2022/4/30.
//

#ifndef CJYPLAYER_PLAYERSHADER_H
#define CJYPLAYER_PLAYERSHADER_H


class PlayerShader {
public:
    virtual bool init();

protected:
    //需要清理，所以在这里定义为成员
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
};


#endif //CJYPLAYER_PLAYERSHADER_H
