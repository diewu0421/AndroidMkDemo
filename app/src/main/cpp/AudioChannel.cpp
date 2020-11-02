//
// Created by zenglingwen on 2020/10/26.
//

#include "AudioChannel.h"

#include <SLES/OpenSLES.h>

#include <SLES/OpenSLES_Android.h>

extern "C" {

#include "libswresample/swresample.h"
}

AudioChannel::AudioChannel(int index , AVCodecContext* avCodecContext) : BaseChannel(index,avCodecContext) {

}

AudioChannel::~AudioChannel() {

}

void AudioChannel::play() {

    SLDataSource source;
}
