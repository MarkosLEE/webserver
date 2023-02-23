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
    const int maxEvents_;//最大事件数
    std::unique_ptr<struct epoll_event[]> events_;//存储事件的数组
    int epollFd_;//epoll句柄
    bool isLT_;//是否是水平触发，即有数据，读事件一直触发。默认为
    bool isOneShot_;//是非注册了oneshot事件
public:
    Epoll(int maxEvents_,bool isLT_,bool isOneShot_):
    maxEvents_(maxEvents_),
    events_(new struct epoll_event[maxEvents_]),
    isLT_(isLT_),
    isOneShot_(isOneShot_){
        epollFd_=epoll_create(1);//初始话时默认监听读事件
    }
    ~Epoll(){
        close(epollFd_);
    }
    void append(const int socketFd);//注册epoll事件
    int wait(){return epoll_wait(epollFd_,events_.get(),maxEvents_,-1);}//等待事件到来
    void del(const int socketFd);//删除监视列表中的某个fd
    void mod(const int socketFd,int option);//重置epoll对socketFd_监听事件
};
#endif