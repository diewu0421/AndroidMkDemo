//
// Created by zenglingwen on 2020/10/22.
//


#include <jni.h>
#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include <queue>
#include <stack>
#include <android/native_window_jni.h>
#include "DNFFmpeg.h"

using namespace std;
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "FFMPEG", __VA_ARGS__);
extern "C" {
#include <libavcodec/avcodec.h>
}

ANativeWindow *window = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef void (*MyCallback)(int);

void myCallback(int a) {

    LOGE("mycallback a = %d", a)
}

void setCallback(MyCallback *callback) {
//    *callback = nullptr;
}

//extern int test();

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_androidmkdemo_MainActivity_myJni(JNIEnv *env, jobject thiz) {

    jclass jcls = env->GetObjectClass(thiz);
    env->GetMethodID(jcls, "reflectMethod", "(Ljava/lang/String;)V");

    env->GetMethodID(jcls, "onPrepare", "()V");

    MyCallback myCallback1 = &myCallback;
    myCallback1(3);
    MyCallback *pointer;
    pointer = &myCallback1;
    setCallback(pointer);

    if (myCallback1 != nullptr) {
        LOGE("not null")
    } else {
        LOGE("null")
    }


//    setCallback();
//    __android_log_print(ANDROID_LOG_ERROR, "zenglw_log", "1111111");
//
//jclass cls = (*env)->GetObjectClass(env, thiz);
//
//jmethodID id = (*env)->GetMethodID(env, cls, "reflectMethod", "(Ljava/lang/String;)V");
////
//char *str = "我是jnit";
//
////    char *str1 = "adsfasf";
//
//char dst[50];

//    strcat(dst, str);
//    strcat(dst, "1111");

//    sprintf(dst, "test方法返回值是 asdf %d", test());

//    __android_log_print(ANDROID_LOG_ERROR, "zenglw", "这是c传过来的值%d", test());

    stack<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);


    LOGE("first = %d", queue.top())
    LOGE("first = %d", queue.top())

    LOGE("first = %d", queue.empty())

    LOGE("这是c传过来的值 %s", av_version_info())

//jstring  s =(*env)->NewStringUTF(env, dst);
//(*env)->CallVoidMethod(env, thiz, id, s);
//(*env)->DeleteLocalRef(env, s);
//    (*env)->ReleaseStringUTFChars(env, s, str);
    return env->NewStringUTF(av_version_info());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_00024Companion_prepare(JNIEnv *env,
                                                                       jobject thiz,
                                                                       jstring play_url) {

}

JavaVM *vm = nullptr;

JavaVM *javaVm = 0;

//int JNI_OnLoad(JavaVM *vm, void *r) {
//    LOGE("JNI_OnLoad111111111")
//    javaVm = vm;
//    return JNI_VERSION_1_6;
//}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_00024Companion_play(JNIEnv *env, jobject thiz) {
}

DNFFmpeg *ffmpeg;


void render(uint8_t *data, int linesize, int width, int height) {

    LOGE("home render")
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }
//设置窗口属性
    ANativeWindow_setBuffersGeometry(window,
                                     width,
                                     height,
                                     WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }
    //填充rgb数据给dst_data
    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);

    int dst_linesize = window_buffer.stride * 4;
    for (int i = 0; i < window_buffer.height; ++i) {
        memcpy(dst_data + i * dst_linesize, data + i * linesize, dst_linesize);
    }

    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);
}

bool flag;
JavaCallHelper *helper = 0;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1prepare(JNIEnv *env, jobject thiz,
                                                                jstring play_url) {
//    if (true) {
//        int *p = 0;
//        *p = 12;
//        return;
//    }

    if (flag) {
        LOGE("已经prepaer过了")
        return;
    }

    const char *playUrl = env->GetStringUTFChars(play_url, JNI_FALSE);

    JavaVM *vm;
    env->GetJavaVM(&vm);

//    JavaCallHelper *helper = new JavaCallHelper(vm, env, thiz);

    helper = new JavaCallHelper(vm, env, thiz);

    ffmpeg = new DNFFmpeg(helper, playUrl);
    ffmpeg->setRenderFrameCallback(render);
    ffmpeg->prepare();
    flag = true;

//    jclass jcls = env->GetObjectClass(thiz);
//    env->DeleteLocalRef(jcls);
    env->ReleaseStringUTFChars(play_url, playUrl);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1start(JNIEnv *env, jobject thiz) {

    if (ffmpeg) {
        LOGE("native start")
    } else {

        LOGE("ffmpeg 为null")
    }

    ffmpeg->start();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1setSurface(JNIEnv *env, jobject thiz,
                                                                   jobject surface) {

    pthread_mutex_lock(&mutex);
    if (window) {
        ANativeWindow_release(window);
        window = nullptr;
    }

    window = ANativeWindow_fromSurface(env, surface);

    pthread_mutex_unlock(&mutex);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1stop(JNIEnv *env, jobject thiz) {
    if (!ffmpeg) {
        LOGE("ffmpeg为null")
        return;
    }
    if (ffmpeg) {
        ffmpeg->stop();
    }

    DELETE(helper)

    DELETE(ffmpeg)
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1release(JNIEnv *env, jobject thiz) {

    pthread_mutex_lock(&mutex);
    if (window) {
        ANativeWindow_release(window);
        window = nullptr;
    }

    pthread_mutex_unlock(&mutex);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1seekTo(JNIEnv *env, jobject thiz,
                                                               jint progress) {
    if (ffmpeg) {
        LOGE("seek %d", progress)
        ffmpeg->seek(progress);
    }
}extern "C"
JNIEXPORT jint JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1get_1duration(JNIEnv *env, jobject thiz) {
    if (ffmpeg) {
        return ffmpeg->getDuration();
    }

    return 0;
}