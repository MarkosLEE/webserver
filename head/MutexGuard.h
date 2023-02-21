#ifndef MUTEXGUARD_H
#define MUTEXGUARD_H
#include"Mutex.h"
#include<exception>
#include<cstdio>
class MutexGuard
{
private:
    /* data */
    Mutex *mutex_;
public:
    MutexGuard()=delete;
    MutexGuard(Mutex& m){
        mutex_=&m;
        if(!mutex_->lock()){
            printf("mutex already locked!\n");
            throw std::exception();
        }
    }
    ~MutexGuard(){
        //析构函数中不应该抛出异常
        mutex_->unlock();
    }
};


#endif