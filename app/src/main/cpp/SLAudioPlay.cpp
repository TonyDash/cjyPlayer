//
// Created by chen jy on 2022/5/2.
//

#include "SLAudioPlay.h"
#include "PlayerLog.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf iplayer = NULL;
static SLAndroidSimpleBufferQueueItf pcmQue = NULL;

SLAudioPlay::SLAudioPlay()
{
    buf = new unsigned char[1024*1024];
}
SLAudioPlay::~SLAudioPlay()
{
    delete buf;
    buf = 0;
}

//static修饰，只在本cpp文件内有效
static SLEngineItf CreateSL()
{
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL,0,0,0,0,0);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->Realize(engineSL,SL_BOOLEAN_FALSE);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->GetInterface(engineSL,SL_IID_ENGINE,&en);
    if(re != SL_RESULT_SUCCESS) return NULL;
    return en;
}


void SLAudioPlay::PlayCall(void *bufq)
{
    if(!bufq)return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf)bufq;
//    LOGE("SLAudioPlay::PlayCall");
    PlayerData d = getData();
    if (d.size<=0){
        LOGE("getData() size is 0");
        return;
    }
    if(!buf)
        return;
    memcpy(buf,d.data,d.size);
    mutex.lock();
    if(pcmQue&&(*pcmQue))
    (*pcmQue)->Enqueue(pcmQue,buf,d.size);
    mutex.unlock();
    d.drop();//这里清理数据，避免访问是发生异常，所以需要复制数据
}


static void PcmCall(SLAndroidSimpleBufferQueueItf bf,void *contex)
{
    SLAudioPlay *ap = (SLAudioPlay *)contex;
    if(!ap)
    {
        LOGE("PcmCall failed contex is null!");
        return;
    }
    ap->PlayCall((void *)bf);
}

void SLAudioPlay::close() {
    IAudioPlay::clear();
    mutex.lock();
    //停止播放
    //需要判断指针不能为空，指针指向的内容也不能为空
    if (iplayer&&(*iplayer)){
        (*iplayer)->SetPlayState(iplayer,SL_PLAYSTATE_STOPPED);
    }
    //清理播放队列
    if (pcmQue&&(*pcmQue)){
        (*pcmQue)->Clear(pcmQue);
    }
    //销毁player对象
    if(player && (*player))
    {
        (*player)->Destroy(player);
    }
    //销毁混音器
    if (mix&&(*mix)){
        (*mix)->Destroy(mix);
    }
    //销毁播放引擎
    if (engineSL &&(*engineSL)){
        (*engineSL)->Destroy(engineSL);
    }
    engineSL = NULL;
    eng = NULL;
    mix = NULL;
    player = NULL;
    iplayer = NULL;
    pcmQue = NULL;
    mutex.unlock();
}


bool SLAudioPlay::StartPlay(PlayerParameter out)
{
    close();
    mutex.lock();
    //1 创建引擎
    eng = CreateSL();
    if(eng)
    {
        LOGD("CreateSL success！ ");
    }
    else
    {
        LOGE("CreateSL failed！ ");
        mutex.unlock();
        return false;
    }

    //2 创建混音器

    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng,&mix,0,0,0);
    if(re !=SL_RESULT_SUCCESS )
    {
        LOGE("SL_RESULT_SUCCESS failed!");
        mutex.unlock();
        return false;
    }
    re = (*mix)->Realize(mix,SL_BOOLEAN_FALSE);
    if(re !=SL_RESULT_SUCCESS )
    {
        LOGE("(*mix)->Realize failed!");
        mutex.unlock();
        return false;
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX,mix};
    SLDataSink audioSink= {&outmix,0};

    //3 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,10};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32) out.channels,//    声道数
            (SLuint32) out.sample_rate*1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataSource ds = {&que,&pcm};


    //4 创建播放器
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng,&player,&ds,&audioSink,sizeof(ids)/sizeof(SLInterfaceID),ids,req);
    if(re !=SL_RESULT_SUCCESS )
    {
        LOGE("CreateAudioPlayer failed!");
        mutex.unlock();
        return false;
    } else{
        LOGD("CreateAudioPlayer success!");
    }
    (*player)->Realize(player,SL_BOOLEAN_FALSE);
    //获取player接口
    re = (*player)->GetInterface(player,SL_IID_PLAY,&iplayer);
    if(re !=SL_RESULT_SUCCESS )
    {
        LOGE("GetInterface SL_IID_PLAY failed!");
        mutex.unlock();
        return false;
    }
    re = (*player)->GetInterface(player,SL_IID_BUFFERQUEUE,&pcmQue);
    if(re !=SL_RESULT_SUCCESS )
    {
        LOGE("GetInterface SL_IID_BUFFERQUEUE failed!");
        mutex.unlock();
        return false;
    }

    //设置回调函数，播放队列空调用
    (*pcmQue)->RegisterCallback(pcmQue,PcmCall,this);

    //设置为播放状态
    (*iplayer)->SetPlayState(iplayer,SL_PLAYSTATE_PLAYING);

    //启动队列回调
    (*pcmQue)->Enqueue(pcmQue,"",1);
    isExit = false;
    LOGI("SLAudioPlay::StartPlay success!");
    mutex.unlock();
    return true;
}

