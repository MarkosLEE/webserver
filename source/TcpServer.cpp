#include"../head/TcpServer.h"
#include<errno.h>
#include<cstdio>
#include<assert.h>
TcpServer::TcpServer(int port_,int maxConnection_,int maxEvents_,const string& name_,const string& address_,int threadNum_):
        port_(port_),
        maxConnection_(maxConnection_),
        maxEvents_(maxEvents_),
        threadNum_(threadNum_),
        buffMaxLength_(1024),
        address_(address_),
        mutex_(),
        name_(name_),
        epollPtr_(std::make_shared<Epoll>(maxEvents_,false)),
        listenConnect_(Socket::socketInit(),0,epollPtr_),
        threadPoolPtr_(std::make_shared<ThreadPool>(name_)),
        connents_()
{
        printf("%d\n",buffMaxLength_);
}
bool TcpServer::start(){
    //完成listenfd对于端口和IP的绑定
    int listenFd=listenConnect_.fd();
    if(listenFd<0){
        printf("listen socket error!\n");
        return false;
    }
    assert(Socket::socketSetPortReuse(listenFd)==0);
    assert(Socket::socketBind(listenFd,address_,port_)==0);
    assert(epollPtr_->append(listenFd,false));
    assert(Socket::socketListen(listenFd,100)==0);
    //启动工作线程池
    if(!threadPoolPtr_->start(threadNum_)){
        printf("threadpool start fail in tcpserver start!\n");
        return false;
    }
    printf("start!\n");
    while (true)
    {
        //printf("wait events\n");
        int count=epollPtr_->wait();
        //printf("count num:%d\n",count);
        for(int i=0; i<count; i++){
            int eventFd=epollPtr_->events_[i].data.fd;
            if(eventFd==listenFd){
                //新的连接
                if(connents_.size()>=maxConnection_){
                    printf("socket connect up to max!\n");
                    continue;
                }
                int newFd=Socket::socketAccept(listenFd);
                if(newFd<0){
                    printf("accept new socket fail\t errno:%d",errno);
                    continue;
                }
                //printf("connect!\n");
                epollPtr_->append(newFd,true);
                connents_.insert(std::make_pair(newFd,std::make_unique<TcpConnect>(newFd,buffMaxLength_,epollPtr_)));
            }
            else if(epollPtr_->events_[i].events&( EPOLLRDHUP | EPOLLHUP | EPOLLERR )){
                //发送三种事件：读关闭 读写都关闭 错误事件，至少发送一件，关闭连接
                printf("socket shutdown\n");
                if(epollPtr_->events_[i].events){
                    printf("events error\n");
                }
                connents_.erase(eventFd);
                printf("erase socketfd:%d!\n",eventFd);
            }
            else if(epollPtr_->events_[i].events&EPOLLIN){
                //读事件
                printf("handleRead\n");
                threadPoolPtr_->run(std::bind(&TcpServer::handleRead,this,eventFd));
            }
            else if(epollPtr_->events_[i].events&EPOLLOUT){
                //写事件
                printf("handleWrite\n");
                threadPoolPtr_->run(std::bind(&TcpServer::handleWrite,this,eventFd));
            }
        }
    }
    return true;
}

void TcpServer::handleRead(const int socketFd){
    if(!connents_[socketFd]->read()){
        printf("read data error!\n");
        connents_.erase(socketFd);
        return;
    }
}

void TcpServer::handleWrite(const int socketFd){

}