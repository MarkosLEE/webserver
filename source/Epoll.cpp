#include"../head/Epoll.h"
#include"../head/Socket.h"
#include<assert.h>
bool Epoll::append(const int socketFd,bool isOneShot_){
    if(currentEventsNum_<maxEventsNum_){
        currentEventsNum_++;
    }
    else return false;
    struct epoll_event ev;
    ev.events=EPOLLIN;
    if(isOneShot_) {
        // 防止同一个通信被不同的线程处理
        ev.events |= EPOLLONESHOT;
    }
    ev.data.fd=socketFd;
    epoll_ctl(epollFd_,EPOLL_CTL_ADD,socketFd,&ev);
    assert(Socket::socketSetNoBlocking(socketFd));
    return true;
}
void Epoll::del(const int socketFd){
    struct epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd=socketFd;
    epoll_ctl(epollFd_,EPOLL_CTL_DEL,socketFd,&ev);
}
void Epoll::mod(const int socketFd,int option){
    struct epoll_event ev;
    ev.data.fd =socketFd ;
    ev.events = option | EPOLLONESHOT | EPOLLRDHUP;
    if(!isLT_){
        ev.events|=EPOLLET;
    }
    epoll_ctl( epollFd_, EPOLL_CTL_MOD, socketFd, &ev );
}