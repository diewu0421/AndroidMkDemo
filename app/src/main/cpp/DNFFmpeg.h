#ifndef PLAYER_DNFFMPEG_H

#define PLAYER_DNFFMPEG_H


#include <cstring>
#include <stdio.h>

extern "C" {
#include <libavformat/avformat.h>
};
#include "macro.h"
#include "JavaHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

class DNFFmpeg {

public:

    DNFFmpeg(JavaHelper *helper, const char *playUrl);

    ~DNFFmpeg();

    void prepare();

    void _prepare();
    void start();

    void test() {

    }
private:
    AudioChannel *audioChannel;
    VideoChannel *videoChannel;
    AVFormatContext *avFormatContext;
    char* source;
    JavaHelper *helper;
    pthread_t pid;
};
#endif
