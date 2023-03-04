#ifndef EPOLL_H
#define EPOLL_H
#include<sys/epoll.h>
#include<vector>
#include<unistd.h>
#include<memory>
class Epoll
{
private:
    /* data */
    const int maxEventsNum_;//最大事件数
    int currentEventsNum_;//当前事件数
    int epollFd_;//epoll句柄
    bool isLT_;//是否是水平触发，即有数据，读事件一直触发。默认为水平触发
public:
    std::unique_ptr<struct epoll_event[]> events_;//存储事件的数组
    Epoll(int maxEventsNum_,bool isLT_=true):
        maxEventsNum_(maxEventsNum_),
        currentEventsNum_(0),
        isLT_(isLT_),
        events_(new struct epoll_event[maxEventsNum_])
    {
        epollFd_=epoll_create(1);//初始话时默认监听读事件
    }
    ~Epoll(){
        close(epollFd_);
    }
    //append只会在TcpServer的start函数中调用，所以是线程安全的
    //由于使用了oneshot所以一个fd同时只能被一个线程操作，关于epoll事件的修改也是线程安全的
    bool append(const int socketFd,bool isOneShot_);//注册epoll读事件
    int wait(){return epoll_wait(epollFd_,events_.get(),maxEventsNum_,-1);}//等待事件到来
    int del(const int socketFd);//删除监视列表中的某个fd
    void mod(const int socketFd,int option);//重置epoll对socketFd_监听事件
};
#endif