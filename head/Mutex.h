#ifndef MUTEX_H
#define MUTEX_H
#include<pthread.h>
#include<exception>
//线程同步，封装互斥锁类(使用pthread)
//todo 使用C++11提供的mutex类替换pthread
class Mutex
{
private:
    /* data */
    pthread_mutex_t mutex_;
public:
    Mutex(){
        if(!pthread_mutex_init(&mutex_,NULL)){
            //初始化锁失败，抛出异常
            throw std::exception();
        }
    }
    ~Mutex(){
        pthread_mutex_destroy(&mutex_);
    }
    bool lock(){
        return !pthread_mutex_lock(&mutex_);
    }
    bool unlock(){
        return !pthread_mutex_unlock(&mutex_);
    }
    pthread_mutex_t*get(){
        return &mutex_;
    }
};

#endif