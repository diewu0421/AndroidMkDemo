//
// Created by Administrator on 2018/9/7.
//

#ifndef PLAYER_BASECHANNEL_H
#define PLAYER_BASECHANNEL_H


#include "safe_queue.h"

extern "C" {
#include <libavcodec/avcodec.h>
};

class BaseChannel {
public:
    BaseChannel(int id, AVCodecContext *avCodecContext, AVRational time_base) :
            time_base(time_base),
            id(id), avCodecContext(avCodecContext) {

        frames.setReleaseCallback(releaseAvFrame);
        packets.setReleaseCallback(releaseAvPacket);
    }

    //virtual
    virtual ~BaseChannel() {
        frames.clear();
        packets.clear();
    }

    /**
     * 释放 AVPacket
     * @param packet
     */
    static void releaseAvPacket(AVPacket** packet) {
        if (packet) {
            av_packet_free(packet);
            //为什么用指针的指针？
            // 指针的指针能够修改传递进来的指针的指向
            *packet = 0;
        }
    }

    static void releaseAvFrame(AVFrame** frame){
        if (frame) {
            av_frame_free(frame);
            //为什么用指针的指针？
            // 指针的指针能够修改传递进来的指针的指向
            *frame = 0;
        }
    }

    //纯虚方法 相当于 抽象方法
    virtual void play() = 0;
    virtual void stop() = 0;

    void clear (){
        packets.clear();
        frames.clear();
    }

    void stopWork() {
        packets.setWork(0);
        frames.setWork(0);
    }

    void startWork() {
        packets.setWork(1);
        frames.setWork(1);
    }

    int id;
    //编码数据包队列
    SafeQueue<AVPacket *> packets;
    //解码数据包队列
    SafeQueue<AVFrame *> frames;
    bool isPlaying;
    AVCodecContext *avCodecContext;
    AVRational time_base;

    double clock;

};


#endif //PLAYER_BASECHANNEL_H
