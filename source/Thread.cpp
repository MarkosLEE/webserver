#include"../head/Thread.h"
#include<pthread.h>
#include<assert.h>
#include<cstdio>
#include<exception>
Thread::Thread(ThreadFunc func_,const string &name):
    func_(std::move(func_)),name_(name_),started_(false),join_(false)
{
        tid_=static_cast<pid_t>(syscall(SYS_gettid));
}
void Thread::start()
{
    started_=true;
    //这里要使用函数指针作为线程函数,所以使用回调机制，在线程函数中回调func_
    if(pthread_create(&this->pthreadId_,NULL,startThread,this)){
        started_=false;
        printf("创建%s线程失败:",this->name_.c_str());
        throw std::exception();
    }
}
void* Thread::startThread(void *thread){
    auto temp=static_cast<Thread*>(thread);
    temp->func_();
}

int Thread::join(){
    assert(started_);
    join_=true;
    return pthread_join(pthreadId_, NULL);//返回0代表成功，返回其他值代表相应的错误
}