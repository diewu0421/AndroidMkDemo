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
#include <pthread.h>
class DNFFmpeg {

public:

    DNFFmpeg(JavaHelper *helper, const char *playUrl);

    ~DNFFmpeg();

    void prepare();

    void _prepare();
    void start();

    void _start();

    void setRenderCallback(RenderCallback callback);
private:
    bool isPlaying;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    AVFormatContext *avFormatContext;
    char* source;
    JavaHelper *helper;
    pthread_t pid;
    pthread_t pid_play;
    RenderCallback renderCallback;
};
#endif
