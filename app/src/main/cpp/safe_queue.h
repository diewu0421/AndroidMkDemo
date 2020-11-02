//
// Created by zenglingwen on 2020/10/22.
//

#ifndef ANDROIDMKDEMO_SAFE_QUEUE_H
#define ANDROIDMKDEMO_SAFE_QUEUE_H

#include <pthread.h>
#include <queue>
#include "macro.h"

using namespace std;

template<typename T>
class SafeQueue {
public:
    typedef void (*MyCallback)(T*);

    typedef void (*SyncHandle)(queue<T> &);

    SafeQueue() {
        pthread_mutex_init(&mutex, 0);
        pthread_cond_init(&cond, 0);
    }

    ~SafeQueue() {

        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
    }

    void push(T t) {
        LOGE("push lock");
        pthread_mutex_lock(&mutex);
        if (work) {
            queue.push(t);
            LOGE("size = %d %d", &queue, queue.size());
            if (queue.size() > 100) {
                LOGE("push signal");
                pthread_cond_signal(&cond);
            }
        }
        pthread_mutex_unlock(&mutex);
        LOGE("push unlock");
    }

    int pop(T& t) {
        LOGE("pop lock");
        pthread_mutex_lock(&mutex);
        int ret = 0;
        while (work && queue.empty()) {
            LOGE("pop wait");
            pthread_cond_wait(&cond, &mutex);
        }
        if (!queue.empty()) {
            t = queue.front();
            LOGE("pop 取出%d %d", &queue, queue.size());
            queue.pop();
            ret = 1;
        }
        pthread_mutex_unlock(&mutex);
        LOGE("pop unlock");
        return ret;
    }

    void clear() {
        pthread_mutex_lock(&mutex);

        while (!queue.empty()) {

            T value = queue.front();
            if (myCallback) {
                myCallback(&value);
            }
            queue.pop();
        }
        pthread_mutex_unlock(&mutex);
    }

    void setReleaseCallback(MyCallback callback) {
        this->myCallback = callback;
    }

    void setSyncHandle(SyncHandle handle) {
        this->handle = handle;
    };

    void sync() {
        pthread_mutex_lock(&mutex);
        handle(&queue);
        pthread_mutex_unlock(&mutex);
    }

    void setWork(int work) {
        pthread_mutex_lock(&mutex);
        this->work = work;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

private:

    pthread_cond_t cond;
    pthread_mutex_t mutex;
    queue<T> queue;
    // 是否工作的标记 1 工作 0 不工作
    int work;
    MyCallback myCallback;
    SyncHandle handle;
};

#endif //ANDROIDMKDEMO_SAFE_QUEUE_H

