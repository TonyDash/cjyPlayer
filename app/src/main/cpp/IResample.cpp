//
// Created by chen jy on 2022/5/1.
//

#include "IResample.h"

void IResample::update(PlayerData data) {
    PlayerData newData = this->resample(data);
    if (newData.size > 0){
        this->notify(newData);
    }
}