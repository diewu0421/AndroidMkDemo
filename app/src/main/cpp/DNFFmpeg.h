#ifndef PLAYER_DNFFMPEG_H

#define PLAYER_DNFFMPEG_H


#include <cstring>
#include <stdio.h>

extern "C" {
#include <libavformat/avformat.h>
};
#include "macro.h"
#include "JavaHelper.h"
class DNFFmpeg {

public:

    DNFFmpeg(JavaHelper *helper, const char *playUrl);

    ~DNFFmpeg();

    void prepare();

    void _prepare();

    void test() {

    }
private:
    AVFormatContext *avFormatContext;
    char* source;
    JavaHelper *helper;
    pthread_t pid;
};
#endif
