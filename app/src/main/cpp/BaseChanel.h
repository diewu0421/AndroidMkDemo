//
// Created by zenglingwen on 2020/10/28.
//

#ifndef ANDROIDMKDEMO_BASECHANEL_H
#define ANDROIDMKDEMO_BASECHANEL_H

#include "safe_queue.h"

extern "C" {
#include "../cmake/include/libavcodec/avcodec.h"
#include "../cmake/include/libswscale/swscale.h"
};

class BaseChannel {

public:
    BaseChannel(int id, AVCodecContext *codecContext) :id(id), avCodecContext(codecContext) {};

    virtual ~BaseChannel() {
//        packets.setCallback(releaseAvPacket);
        packets.setReleaseCallback(BaseChannel::releaseAvPacket);
        packets.clear();
    }

    static void releaseAvPacket(AVPacket **packet){
        if (packet && *packet) {
            av_packet_free(packet);
            /*
             * 为什么要用指针的指针？
             * 指针的指针能够修改传递进来的指针的指向
             */
            *packet = 0;
        }
    }
    static void releaseAvFrames(AVFrame **avFrame) {
        if (avFrame && *avFrame) {
            av_frame_free(avFrame);
            *avFrame = 0;
        }
    }
    virtual void play() = 0;

    int id;
    SafeQueue<AVPacket *> packets;
    AVCodecContext *avCodecContext;

};
#endif //ANDROIDMKDEMO_BASECHANEL_H
