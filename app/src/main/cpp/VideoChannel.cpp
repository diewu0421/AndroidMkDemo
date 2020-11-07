//
// Created by Administrator on 2018/9/5.
//

extern "C"{
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
}
#include "VideoChannel.h"
#include "macro.h"
#define DELAY_CONST 1000000

void *decode_task(void *args) {
    VideoChannel *channel = static_cast<VideoChannel *>(args);
    channel->decode();
    return 0;
}

void *render_task(void *args) {
    VideoChannel *channel = static_cast<VideoChannel *>(args);
    channel->render();
    return 0;
}

void dropFrameCallback(queue<AVFrame *> &queue) {

    if (!queue.empty()) {
        AVFrame *frame = queue.front();
        BaseChannel::releaseAvFrame(&frame);
        queue.pop();
    }
}
VideoChannel::VideoChannel(int id, AVCodecContext *avCodecContext, AVRational time_base, int fps)
        : BaseChannel(
        id, avCodecContext, time_base) {

    this->fps = fps;
    frames.setSyncHandle(dropFrameCallback);
}


VideoChannel::~VideoChannel() {
    LOGE("videochannel xigou before %d %d", frames.size(), packets.size());

    frames.clear();
    frames.setSyncHandle(0);
    frames.setWork(0);
    frames.setReleaseCallback(0);

    packets.clear();
    packets.setSyncHandle(0);
    packets.setWork(0);
    packets.setReleaseCallback(0);
    LOGE("videochannel xigou after");
}

void VideoChannel::play() {
    isPlaying = 1;
    frames.setWork(1);
    packets.setWork(1);
    //1、解码
    pthread_create(&pid_decode, 0, decode_task, this);
    //2、播放
    pthread_create(&pid_render, 0, render_task, this);
}

//解码
void VideoChannel::decode() {
    AVPacket *packet = 0;
    while (isPlaying) {
        //取出一个数据包
        int ret = packets.pop(packet);
        if (!isPlaying) {
            break;
        }
        //取出失败
        if (!ret) {
            continue;
        }
        //把包丢给解码器
        ret = avcodec_send_packet(avCodecContext, packet);
        releaseAvPacket(&packet);
        //重试
        if (ret != 0) {
            break;
        }
        //代表了一个图像 (将这个图像先输出来)
        AVFrame *frame = av_frame_alloc();
        //从解码器中读取 解码后的数据包 AVFrame
        ret = avcodec_receive_frame(avCodecContext, frame);
        //需要更多的数据才能够进行解码
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if(ret != 0){
            break;
        }
        //再开一个线程 来播放 (流畅度)
        frames.push(frame);
    }
    releaseAvPacket(&packet);
}

//播放
void VideoChannel::render() {
    //目标： RGBA
    swsContext = sws_getContext(
            avCodecContext->width, avCodecContext->height,avCodecContext->pix_fmt,
            avCodecContext->width, avCodecContext->height,AV_PIX_FMT_RGBA,
            SWS_BILINEAR,0,0,0);

    double frame_delay = 1.0 / fps;
    AVFrame* frame = 0;
    //指针数组
    uint8_t *dst_data[4];
    int dst_linesize[4];
    av_image_alloc(dst_data, dst_linesize,
                   avCodecContext->width, avCodecContext->height,AV_PIX_FMT_RGBA, 1);
    while (isPlaying){
        int ret = frames.pop(frame);
        if (!isPlaying){
            break;
        }
        //src_linesize: 表示每一行存放的 字节长度
        sws_scale(swsContext, reinterpret_cast<const uint8_t *const *>(frame->data),
                  frame->linesize, 0,
                  avCodecContext->height,
                  dst_data,
                  dst_linesize);

        double clock = frame->best_effort_timestamp * av_q2d(time_base);
        double extra_delay = frame->repeat_pict / (2 * fps);
        double real_delay = frame_delay + extra_delay;
        LOGE("延时 %lf", real_delay * DELAY_CONST);
#if 1
        if (!audioChannel) {
            //回调出去进行播放
            av_usleep(real_delay * DELAY_CONST);

        } else {
            if (clock == 0) {
                av_usleep(real_delay * DELAY_CONST);
            } else {

                double diff = clock - audioChannel->clock;

                if (diff > 0) {
                    // 视频快了

                    LOGE("视频快了%lf", diff);
                    av_usleep((real_delay + diff) * DELAY_CONST);
                } else {
                    // 视频慢了
                    LOGE("视频慢了%lf", diff);
                    double audioClock = audioChannel->clock;
                    if (fabs(clock - audioClock) >= 0.05) {
                        // 丢包
                        releaseAvFrame(&frame);
                        frames.sync();
                        continue;
                    }
                }
            }
        }

#endif

        callback(dst_data[0],dst_linesize[0],avCodecContext->width, avCodecContext->height);
        releaseAvFrame(&frame);
    }
    av_freep(&dst_data[0]);
    releaseAvFrame(&frame);
}

void VideoChannel::setRenderFrameCallback(RenderFrameCallback callback) {
    this->callback = callback;
}

void VideoChannel::setAudioChannel(AudioChannel *channel) {
    this->audioChannel = channel;

}

void VideoChannel::stop() {
    isPlaying = 0;
    frames.setWork(0);
    packets.setWork(0);

    pthread_join(pid_decode, 0);
    pthread_join(pid_render, 0);
    if (swsContext) {
        sws_freeContext(swsContext);
        swsContext = 0;
    }

}
