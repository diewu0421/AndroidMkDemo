//
// Created by Administrator on 2018/9/5.
//

#ifndef PLAYER_DNFFMPEG_H
#define PLAYER_DNFFMPEG_H

#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

extern  "C" {
#include <libavformat/avformat.h>
}


class DNFFmpeg {
public:
    DNFFmpeg(JavaCallHelper* callHelper,const char* dataSource);
    ~DNFFmpeg();

    void prepare();
    void _prepare();

    void start();
    void _start();

    void setRenderFrameCallback(RenderFrameCallback callback);

    void stop();

public:
    char *dataSource;
    pthread_t pid;
    pthread_t pid_play;
    AVFormatContext *formatContext = 0;
    JavaCallHelper* callHelper;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    RenderFrameCallback callback;
    bool isPlaying;
    pthread_t stop_pid;
};


#endif //PLAYER_DNFFMPEG_H
