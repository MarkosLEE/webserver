#ifndef TCPCONNECT_H
#define TCPCONNECT_H
#include"../head/Socket.h"
#include"../head/Epoll.h"
#include<memory>
class TcpConnect
{
private:
    const int socketFd_;
    int buffMaxLength_;
    std::unique_ptr<char[]>buff_;
    int buffCurentLength_;
    char* buffPtr_;
    std::shared_ptr<Epoll> epoll_;
public:
    TcpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_);
    ~TcpConnect(){
        epoll_->del(socketFd_);
        Socket::socketClose(socketFd_);
    }
    bool read();//从缓冲区读数据
    bool wirte();//往缓冲区写数据
    bool wirtev(struct iovec* iv,int ivCount);//聚集写
    char*get();//返回缓冲区指针，用于解析报文和生成报文
    int fd(){return socketFd_;}
};
#endif