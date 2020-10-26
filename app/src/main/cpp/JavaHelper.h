//
// Created by zenglingwen on 2020/10/26.
//

#ifndef ANDROIDMKDEMO_JAVAHELPER_H
#define ANDROIDMKDEMO_JAVAHELPER_H


#include <jni.h>
class JavaHelper {
public:

    JavaHelper(JNIEnv *env, JavaVM *vm, jobject instance);

    ~JavaHelper();

    void onError(int pthreadId, int errorCode);

private:

    JNIEnv *env;
    JavaVM *vm;
    jobject  instance;
    jmethodID onErrorId;
    jmethodID onPrepareId;
};
#endif //ANDROIDMKDEMO_JAVAHELPER_H
