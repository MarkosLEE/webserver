#ifndef TCPCONNECT_H
#define TCPCONNECT_H
#include"../head/Socket.h"
#include"../head/Epoll.h"
#include<memory>
class TcpConnect
{
protected:
    const int socketFd_;
    int buffMaxLength_;
    std::unique_ptr<char[]>buff_;
    int buffCurentLength_;
    std::shared_ptr<Epoll> epoll_;
    std::unique_ptr< struct iovec[]> iv_;//指向要发送的资源文件
    int ivCount_;//要发送的资源文件数量
    bool isReady_;//是否已准备好发送
public:
    TcpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_);
    ~TcpConnect(){
        epoll_->del(socketFd_);
        Socket::socketClose(socketFd_);
    }
    bool read();//从缓冲区读数据
    bool wirte();//往缓冲区写数据
    bool wirtev();//聚集写
    char*get();//返回缓冲区指针，用于解析报文和生成报文
    int fd(){return socketFd_;}
    void setReady(bool flag);
    bool isReady();
};
#endif