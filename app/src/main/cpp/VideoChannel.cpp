//
// Created by zenglingwen on 2020/10/26.
//

#include "VideoChannel.h"
#include "macro.h"

extern "C" {

#include "libavutil/imgutils.h"

}


VideoChannel::VideoChannel(int id, AVCodecContext *context) : BaseChannel(id, context) {

    avFrames.setCallback(releaseAvFrames);
}

VideoChannel::~VideoChannel() {
    avFrames.clear();
}

void* decode_task(void* arg) {

    LOGE("Deocde task0");
    VideoChannel * channel = static_cast<VideoChannel *>(arg);
    channel->decode();
    return 0;
}

void* render_task(void* arg) {
    LOGE("render_task");
    VideoChannel * channel = static_cast<VideoChannel *>(arg);
    channel->render();
    return 0;
}

void VideoChannel::play() {
    isPlaying = true;
    LOGE("play");
    avFrames.setWork(1);
    packets.setWork(1);
    // 解码
    pthread_create(&decode_pid, 0, decode_task, this);
    pthread_create(&render_pid, 0, render_task, this);

    // 播放
}

void VideoChannel::decode() {

    AVPacket *packet = 0;
    while (isPlaying) {

        int ret = packets.pop(packet);

        if (!isPlaying) {
            break;
        }

        if (!ret) {
            continue;
        }

        ret = avcodec_send_packet(avCodecContext, packet);
        releaseAvPacket(&packet);
        if (ret != 0) {
           break;
        }
        // 获得一个图像
        AVFrame *avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);

        if (ret == AVERROR_EOF) {
            continue;
        } else if (ret != 0) {
            break;
        }

        // 再开一个线程
        avFrames.push(avFrame);

    }
    releaseAvPacket(&packet);
}

void VideoChannel::render() {
    swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                avCodecContext->pix_fmt,
                                avCodecContext->width, avCodecContext->height, AV_PIX_FMT_RGBA,
                                SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVFrame *frame = 0;
    uint8_t *dst_data[4];
    int dst_linesize[4];
    LOGE("render imple %d" ,isPlaying );
    av_image_alloc(dst_data, dst_linesize,
                   avCodecContext->width,
                   avCodecContext->height,
                   AV_PIX_FMT_RGBA, 1
                   );
    while (isPlaying) {
        int ret = avFrames.pop(frame);

        if (!isPlaying) {
            break;
        }

        // demo中没有这个
        if (ret == 0) {
            continue;
        }
        sws_scale(swsContext, frame->data,
                  frame->linesize, 0,
                  avCodecContext->height,
                  dst_data, dst_linesize);

        // 回调出去进行播放
        callback(dst_data[0], dst_linesize[0], avCodecContext->width, avCodecContext->height);
        releaseAvFrames(&frame);
    }

    av_free(&dst_data[0]);
    releaseAvFrames(&frame);


}

void VideoChannel::setRenderCallback(RenderCallback callback) {
    this->callback = callback;
}
