//
// Created by zenglingwen on 2020/10/23.
//


#include <pthread.h>
#include "DNFFmpeg.h"



void* task_prepare(void *arg) {
    DNFFmpeg *dnfFmpeg = static_cast<DNFFmpeg *>(arg);
    dnfFmpeg->_prepare();
    return nullptr;
}

DNFFmpeg::DNFFmpeg(JavaHelper *helper, const char *playUrl) {
    this->helper = helper;
    this->source = new char[strlen(playUrl)];
    strcpy(this->source, playUrl);
}

DNFFmpeg::~DNFFmpeg() {
    DELETE(source)
    DELETE(helper)
}

void DNFFmpeg::_prepare() {
    avformat_network_init();
    int ret = 0;
    avFormatContext = 0;
//    ret = avformat_open_input(&avFormatContext, source, 0, 0);
//    if (ret != 0) {
//        helper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
//        return;
//    }
//
//    ret = avformat_find_stream_info(avFormatContext, 0);
//    if (ret < 0) {
//        helper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAMS);
//        return;
//    }
//    unsigned int streams = avFormatContext->nb_streams;
//    LOGE("stream = %d" , streams);
//    avformat_close_input(&avFormatContext);
//    LOGE("close file");
}

void DNFFmpeg::prepare() {
    pthread_create(&pid, nullptr, task_prepare, this);
}
