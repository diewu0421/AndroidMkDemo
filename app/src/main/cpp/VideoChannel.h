//
// Created by zenglingwen on 2020/10/26.
//

#ifndef ANDROIDMKDEMO_VIDEOCHANNEL_H
#define ANDROIDMKDEMO_VIDEOCHANNEL_H

#include "BaseChanel.h"

typedef void* (*RenderCallback)(uint8_t *, int, int, int);
class  VideoChannel : public BaseChannel{
public:
    VideoChannel(int id, AVCodecContext* context);

    ~VideoChannel();

    void setRenderCallback(RenderCallback callback1);

    void play();
    void decode();
    void render();
    pthread_t decode_pid;
    pthread_t render_pid;
    bool isPlaying;

    SafeQueue<AVFrame *> avFrames;
    SwsContext *swsContext;
    RenderCallback callback;
};
#endif //ANDROIDMKDEMO_VIDEOCHANNEL_H
