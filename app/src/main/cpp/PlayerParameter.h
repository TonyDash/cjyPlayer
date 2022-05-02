//
// Created by chen jy on 2022/4/28.
//

#ifndef CJYPLAYER_PLAYERPARAMETER_H
#define CJYPLAYER_PLAYERPARAMETER_H

//只声明，不用管实现
struct AVCodecParameters;

class PlayerParameter {
public:
    AVCodecParameters *para = 0 ;
    int channels = 2;
    int sample_rate = 44100;
};


#endif //CJYPLAYER_PLAYERPARAMETER_H
