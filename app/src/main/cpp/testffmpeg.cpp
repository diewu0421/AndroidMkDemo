//
// Created by zenglingwen on 2020/10/22.
//


#include <jni.h>
#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include <queue>
#include <stack>

#include "DNFFmpeg.h"

using namespace std;
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "zenglw", __VA_ARGS__);
extern "C" {
#include <libavcodec/avcodec.h>
}


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

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1prepare(JNIEnv *env, jobject thiz,
                                                                jstring play_url) {

    const char *playUrl = env->GetStringUTFChars(play_url, JNI_FALSE);

    JavaVM *vm;
    env->GetJavaVM(&vm);

    JavaHelper *helper = new JavaHelper(env, vm, thiz);

    ffmpeg = new DNFFmpeg(helper, playUrl);
    ffmpeg->prepare();

//    jclass jcls = env->GetObjectClass(thiz);
//    env->DeleteLocalRef(jcls);
    env->ReleaseStringUTFChars(play_url, playUrl);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1start(JNIEnv *env, jobject thiz) {


}extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_utils_DNFFPlayer_native_1setSurface(JNIEnv *env, jobject thiz,
                                                                   jobject surface) {

}