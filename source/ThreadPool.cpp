#include"../head/ThreadPool.h"
#include"../head/MutexGuard.h"
#include<cstdio>
ThreadPool::ThreadPool(const string& name):
    threadNum_(0),
    maxRequests_(100),
    running_(false),
    mutex_(),
    notEmpty_(),
    notFull_(),
    name_(name){}
ThreadPool::~ThreadPool(){
    if(running_){
        stop();
    }
}
void ThreadPool::stop(){

    MutexGuard lock(mutex_);
    running_=false;
    notEmpty_.notifyAll();
    notFull_.notifyAll();
    for(auto v=threads_.begin(); v!=threads_.end(); v++){
        (*v)->join();
    }
    threads_.clear();
    queue_.clear();
}
//线程安全
void ThreadPool::run(Task task){
    if(threads_.empty()){
        task();//如果线程池中不存在子线程，则当前线程执行任务
    }
    else{
        MutexGuard lock(mutex_);//可能有多个线程调用线程池对象，所以这里要用锁保护起来
        while(running_&&isFull()){//这里存在一种可能，即任务队列notfull，但是线程池被stopped了，所以要检查running_的状态
            notFull_.wait(mutex_);//等待，直到不满
        }
        if(!running_){
            return;//说明线程池中有线程，但是线程池被stopped，所以对于新增任务不做处理直接退出
        }
        queue_.push_back(std::move(task));//使用std::move减少拷贝
        notEmpty_.notify();
    }
}
void ThreadPool::threadRun(){
    while(running_){
        Task task(take());
        if(task){
            task();
        }                        
    }
}
ThreadPool::Task ThreadPool::take(){
    MutexGuard lock(mutex_);
    while(isEmpty()&&running_){
        notEmpty_.wait(mutex_);
    }
    if(!isEmpty()){
        Task task=queue_.front();
        queue_.pop_front();
        if(!isFull()){
            notFull_.notify();
        }
        return task;
    }
    return NULL;

}

bool ThreadPool::start(const int threadNum){
    //要防止重复start线程池
    MutexGuard lock(mutex_);
    if(running_){
        return false;
    }
    running_=true;
    threadNum_=threadNum;
    threads_.reserve(threadNum_);
    for(size_t i=0; i<threadNum_; i++){
        threads_.emplace_back(new Thread(std::bind(&ThreadPool::threadRun,this),name_+std::to_string(i)));
        threads_[i]->start(); 
    }
    return true;
}