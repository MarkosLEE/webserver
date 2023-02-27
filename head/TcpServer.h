#ifndef TcpServer_H
#define TcpServer_H
#include"TcpConnect.h"
#include"Mutex.h"
#include"Epoll.h"
#include"ThreadPool.h"
#include"Type.h"
#include<map>
#include<memory>
class TcpServer
{
protected:
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
    std::unique_ptr<ThreadPool> threadPoolPtr_;//线程池
public:
    TcpServer(int port_,int maxConnection_,int maxEvents_,const string& name_,const string& address_,int threadNum_);
    virtual ~TcpServer()=0;//纯虚函数也可以定义
    virtual bool start()=0;//启动服务器
    virtual void handleRead(const int fd)=0;//处理读事件
    virtual void handleWrite(const int fd)=0;//处理写事件
    virtual void handleParseHttp(const int fd)=0;//解析http报文
};

#endif