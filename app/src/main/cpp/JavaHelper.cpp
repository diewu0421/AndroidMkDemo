//
// Created by zenglingwen on 2020/10/26.
//

#include "JavaHelper.h"
#include "macro.h"

void JavaHelper::onError(int pthreadId, int errorCode) {

    if (pthreadId == THREAD_MAIN) {
        env->CallVoidMethod(instance, onErrorId, errorCode);
    } else {
        JNIEnv *env = 0;
        int status;

        bool isAttached = false;

        status = vm->GetEnv((void **) &env, JNI_VERSION_1_6);
        if (status < 0) {
            // 获取属于我这一个线程的env
            int attachStatus = vm->AttachCurrentThread(&env, 0);
            if (attachStatus) {
                return;
            }
            isAttached = true;
        }


        env->CallVoidMethod(instance, onErrorId, errorCode);
        if (isAttached) {
            vm->DetachCurrentThread();
        } else {
        }
//        jint detachId = vm->DetachCurrentThread();
//        LOGE("分离当前线程%d", detachId);

    }

}

JavaHelper::JavaHelper(JNIEnv *env, JavaVM *vm, jobject instance) {
    this->env = env;
    this->vm = vm;
    this->instance = env->NewGlobalRef(instance);

    jclass jcls = env->GetObjectClass(instance);
    onErrorId = env->GetMethodID(jcls, "onError", "(I)V");
    onPrepareId = env->GetMethodID(jcls, "onPrepare", "()V");

    LOGE("22222222");
}

JavaHelper::~JavaHelper() {
    env->DeleteGlobalRef(instance);

}

void JavaHelper::onPrepare(int pthreadId) {

    if (pthreadId == THREAD_MAIN) {
        env->CallVoidMethod(instance, onPrepareId);

    } else {
        JNIEnv *env = 0;
        int status;

        bool isAttached = false;

        status = vm->GetEnv((void **) &env, JNI_VERSION_1_6);
        if (status < 0) {
            // 获取属于我这一个线程的env
            int attachStatus = vm->AttachCurrentThread(&env, 0);
            if (attachStatus) {
                return;
            }
            isAttached = true;
        }


        env->CallVoidMethod(instance, onPrepareId);
        if (isAttached) {
            vm->DetachCurrentThread();
        } else {
        }
    }
}
