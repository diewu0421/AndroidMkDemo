//
// Created by zenglingwen on 2020/10/26.
//

#ifndef ANDROIDMKDEMO_AUDIOCHANNEL_H
#define ANDROIDMKDEMO_AUDIOCHANNEL_H

#include "BaseChanel.h"

class AudioChannel : public BaseChannel {


public:
    AudioChannel(int index, AVCodecContext *avCodecContext);

    ~AudioChannel();

    void play();

private:
    int index;
};
#endif //ANDROIDMKDEMO_AUDIOCHANNEL_H
