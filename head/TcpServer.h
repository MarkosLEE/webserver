#ifndef TCPSERVER_H
#define TCPSERVER_H
#include"TcpConnect.h"
#include"Mutex.h"
#include"Epoll.h"
#include"ThreadPool.h"
#include"Type.h"
#include<map>
#include<memory>
class TcpServer
{
private:
    const int  port_;//服务器监听的端口
    const size_t  maxConnection_;//最大连接数
    const int  maxEvents_;//最大事件数
    const int  threadNum_;
    const int  buffMaxLength_;
    const string address_;//IP地址
    Mutex mutex_;
    const string name_;//服务器的名字，便于日志
    std::shared_ptr<Epoll> epollPtr_;
    TcpConnect listenConnect_;//监听socket
    std::shared_ptr<ThreadPool> threadPoolPtr_;//线程池
    std::map<int,std::unique_ptr<TcpConnect>>connents_;//fd作为键
public:
    TcpServer(int port_,int maxConnection_,int maxEvents_,const string& name_,const string& address_,int threadNum_);
    ~TcpServer(){};
    bool start();//启动服务器
    void handleRead(const int fd);//处理读事件
    void handleWrite(const int fd);//处理写事件
};

#endif