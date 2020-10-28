//
// Created by zenglingwen on 2020/10/22.
//

#ifndef ANDROIDMKDEMO_SAFE_QUEUE_H
#define ANDROIDMKDEMO_SAFE_QUEUE_H

#include <pthread.h>
#include <queue>

using namespace std;

template<typename T>
class SafeQueue {
public:
    typedef void (*MyCallback)(T*);

    typedef void (*SyncHandle)(queue<T> &);

    SafeQueue() {
        pthread_mutex_init(mutex, 0);
        pthread_cond_init(cond, 0);
    }

    ~SafeQueue() {

        pthread_mutex_destroy(mutex, 0);
        pthread_cond_destroy(cond, 0);
    }

    void push(T t) {
        pthread_mutex_lock(mutex);
        if (work) {
            queue.push(t);
        }
        pthread_mutex_unlock(mutex);
        pthread_cond_signal(cond);
    }

    int pop(T& t) {
        int ret = 0;
        pthread_mutex_lock(mutex);
        while (work && queue.empty()) {
            pthread_cond_wait(cond, 0);
        }
        if (!queue.empty()) {
            t = queue.front();
            queue.pop();
            ret = 1;
        }
        pthread_mutex_unlock(mutex);
        return ret;
    }

    void clear() {
        pthread_mutex_lock(mutex);

        while (!queue.empty()) {

            T value = queue.front();
            if (myCallback) {
                myCallback(&value);
            }
            queue.pop();
        }
        pthread_mutex_unlock(mutex);
    }

    void setCallback(MyCallback callback) {
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
        pthread_mutex_unlock(&mutex);
    }

private:

    pthread_cond_t &cond;
    pthread_mutex_t &mutex;
    queue<T> queue;
    // 是否工作的标记 1 工作 0 不工作
    int work;
    MyCallback myCallback;
    SyncHandle handle;
};

#endif //ANDROIDMKDEMO_SAFE_QUEUE_H

