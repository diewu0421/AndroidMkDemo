//
// Created by zenglingwen on 2020/10/23.
//


#include "DNFFmpeg.h"



void* task_prepare(void *arg) {
    DNFFmpeg *dnfFmpeg = static_cast<DNFFmpeg *>(arg);
    dnfFmpeg->_prepare();
    return nullptr;
}

DNFFmpeg::DNFFmpeg(JavaHelper *helper, const char *playUrl) {
    this->helper = helper;
    this->source = new char[strlen(playUrl + 1)];
    strcpy(this->source, playUrl);
}

DNFFmpeg::~DNFFmpeg() {
    DELETE(source)
    DELETE(helper)
}

void DNFFmpeg::_prepare() {
//    av_register_all();
    avformat_network_init();
    int ret = 0;
    avFormatContext = 0;
    LOGE("视频url %s", source);
    ret = avformat_open_input(&avFormatContext, source, 0, 0);
    if (ret != 0) {
        LOGE("打开文件失败1 %s", av_err2str(ret));
        helper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        return;
    }

    ret = avformat_find_stream_info(avFormatContext, 0);
    if (ret < 0) {
        LOGE("打开文件失败2 %s", av_err2str(ret));
        helper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAMS);
        return;
    }
    unsigned int streams = avFormatContext->nb_streams;

    for (int i = 0; i < streams; ++i) {
        AVStream *avStream = avFormatContext->streams[i];
        AVCodecParameters *parameters = avStream->codecpar;
        // 获取解码器
        AVCodec *avCodec = avcodec_find_decoder(parameters->codec_id);

        if (avCodec == NULL) {
            LOGE("查找解码器失败:%s",av_err2str(ret));

            helper->onError(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            return;
        }
        AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
        if (avCodecContext == nullptr) {
            LOGE("创建解码上下文失败:%s",av_err2str(ret));
            helper->onError(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            return;
        }
        ret = avcodec_parameters_to_context(avCodecContext, parameters);
        if (ret < 0) {
            LOGE("设置解码上下文参数失败:%s",av_err2str(ret));

            helper->onError(THREAD_CHILD, FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            return;
        }

        ret = avcodec_open2(avCodecContext, avCodec, 0);
        if (ret < 0) {
            LOGE("打开解码器失败:%s",av_err2str(ret));

            helper->onError(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        if (parameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioChannel = new AudioChannel(i);

        } else if (parameters->codec_type== AVMEDIA_TYPE_VIDEO) {
            videoChannel = new VideoChannel(i);

        }
    }

    if (!audioChannel && !videoChannel) {
        LOGE("没有音视频");
        helper->onError(THREAD_CHILD, FFMPEG_NOMEDIA);
        return;
    }

    avformat_close_input(&avFormatContext);
    helper->onPrepare(THREAD_CHILD);
    LOGE("close file");
}

void DNFFmpeg::prepare() {
    pthread_create(&pid, nullptr, task_prepare, this);
}


void *play_task(void *arg) {
    DNFFmpeg *dnfFmpeg = static_cast<DNFFmpeg *>(arg);
    dnfFmpeg->_start();

}
void DNFFmpeg::start() {
    isPlaying = true;
    pthread_create(&pid_play, nullptr, play_task, this);

}

void DNFFmpeg::_start() {
    // 读取媒体数据包
    int ret;
    while (isPlaying) {

        AVPacket *packet = av_packet_alloc();
        ret = av_read_frame(avFormatContext, packet);
        if (ret == 0) {
            // stream_index 这一个流的一个序号
            if (audioChannel && packet->stream_index == audioChannel->id) {

            } else if (videoChannel && packet->stream_index == videoChannel->id) {
                videoChannel->packets.push(packet);
            }

        } else if (ret == AVERROR_EOF) {

        } else {

        }
    }

    // 解码
}

