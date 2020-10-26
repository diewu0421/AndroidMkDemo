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
    typedef void (*MyCallback)(T &);

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
        queue.push(t);
        pthread_cond_signal(cond);
        pthread_mutex_unlock(mutex);
    }

    int pop(T& t) {
        int ret = 0;
        pthread_mutex_lock(mutex);
        while (queue.empty()) {
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
                myCallback(value);
            }
            queue.pop();
        }
        pthread_mutex_unlock(mutex);
    }

    void setCallback(MyCallback callback) {
        this->myCallback = callback;
    }

private:

    pthread_cond_t &cond;
    pthread_mutex_t &mutex;
    queue<T> queue;
    MyCallback myCallback;
};

#endif //ANDROIDMKDEMO_SAFE_QUEUE_H

