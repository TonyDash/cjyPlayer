//
// Created by chen jy on 2022/5/1.
//

#include "IResample.h"
#include "PlayerLog.h"

void IResample::update(PlayerData data) {
    PlayerData newData = this->resample(data);
    LOGD("IResample::update %d",newData.size);
    if (newData.size > 0){
        this->notify(newData);
    }
}