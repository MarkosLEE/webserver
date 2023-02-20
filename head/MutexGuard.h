#ifndef MUTEXGUARD_H
#define MUTEXGUARD_H
#include"Mutex.h"
#include<exception>
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
            throw std::exception();
        }
    }
    ~MutexGuard(){
        //析构函数中不应该抛出异常
        mutex_->unlock();
    }
};

MutexGuard::MutexGuard(/* args */)
{
}

MutexGuard::~MutexGuard()
{
}

#endif