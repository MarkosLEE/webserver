#ifndef CONDITION_H
#define CONDITION_H
#include"Mutex.h"
#include<pthread.h>
#include<exception>
//封装条件变量类
class Condition
{
private:
    pthread_cond_t cond_;
public:
    Condition(){
        if(!pthread_cond_init(&cond_,NULL)){
            throw std::exception();
        }
    }
    ~Condition(){
        pthread_cond_destroy(&cond_);
    }
    bool wait(Mutex&m){
        return !pthread_cond_wait(&cond_,m.get());
    }
    bool waitForTime(Mutex&m,const timespec&t){
        return !pthread_cond_timedwait(&cond_,m.get(),&t);
    }
    bool notify(){
        return !pthread_cond_signal(&cond_);
    }
    bool notifyAll(){
        return !pthread_cond_broadcast(&cond_);
    }
};
#endif 