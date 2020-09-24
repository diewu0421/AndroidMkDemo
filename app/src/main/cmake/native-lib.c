//
// Created by zenglingwen on 2020/9/15.
//

#include <jni.h>
#include <android/log.h>
#include <string.h>
#include <stdio.h>

extern int test();

JNIEXPORT void JNICALL
Java_com_example_androidmkdemo_MainActivity_myJni(JNIEnv *env, jobject thiz) {

//    __android_log_print(ANDROID_LOG_ERROR, "zenglw_log", "1111111");

    jclass cls = (*env)->GetObjectClass(env, thiz);

    jmethodID id = (*env)->GetMethodID(env, cls, "reflectMethod", "(Ljava/lang/String;)V");
//
    char *str = "我是jnit";

//    char *str1 = "adsfasf";

    char dst[50];

//    strcat(dst, str);
//    strcat(dst, "1111");

    sprintf(dst, "test方法返回值是 asdf %d", test());

    jstring  s =(*env)->NewStringUTF(env, dst);
    (*env)->CallVoidMethod(env, thiz, id, s);
    (*env)->DeleteLocalRef(env, s);
//    (*env)->ReleaseStringUTFChars(env, s, str);
}
