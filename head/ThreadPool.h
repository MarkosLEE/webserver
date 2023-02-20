#ifndef THREADPOOL_H
#define THREADPOOL_H
#include"Mutex.h"
#include"Condition.h"
#include"Type.h"
#include"Thread.h"
#include<pthread.h>
#include<vector>
#include<memory>
#include<deque>
class ThreadPool
{
    typedef std::function<void()> Task;
private:
    size_t threadNum_;//线程池中线程的个数
    size_t maxRequests_;//任务队列中允许的最大等待数量
    bool running_;//当前线程池是否启动
    Mutex mutex_;//当前线程池的互斥量
    Condition notEmpty_;//任务队列非空
    Condition notFull_;//任务队列非满
    const string name_;//线程池的名字，用于日志记录 
    std::vector<std::unique_ptr<Thread>> threads_;                   //线程池中的线程
    std::deque<Task> queue_;                   //任务队列

public:
    //不应该一次传太多参数，应该使用set函数设置
    ThreadPool(const string&);
    ~ThreadPool();
    bool start(const int threadNum);//设置线程池线程数量并启动线程，等待任务进入
    void threadRun();//线程工作函数，使用bind绑定传给thread
    void run(Task task);//运行任务，将任务放入任务队列，然后通知线程
    void stop();//析构时，调用，确保线程池所有线程已结束
    bool isFull()const{return maxRequests_>0 && queue_.size()>=maxRequests_;}//任务队列是否已满
    bool isEmpty()const{return queue_.empty();}//任务队列是否为空
    Task take();//获取任务
};


#endif