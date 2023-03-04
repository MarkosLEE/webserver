#ifndef TCPCONNECT_H
#define TCPCONNECT_H
#include"../head/Socket.h"
#include"../head/Epoll.h"
#include<memory>
typedef std::shared_ptr<char[]> SharedBuffPtr;
class TcpConnect
{
protected:
    const int socketFd_;
    int buffMaxLength_;
    SharedBuffPtr buff_;
    int buffCurentLength_;//缓冲区当初的数据长度
    int buffReadPos_;//读缓冲区的指针当前所处的位置
    std::shared_ptr<Epoll> epoll_;//线程安全
    std::unique_ptr< struct iovec[]> iv_;//指向要发送的资源文件
    int ivCount_;//要发送的资源文件数量
    bool isReady_;//是否已准备好发送
public:
    TcpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_);
    ~TcpConnect();
    bool read();//从缓冲区读数据
    bool wirte();//往缓冲区写数据
    bool wirtev();//聚集写
    char*get();//返回缓冲区指针，用于解析报文和生成报文
    int fd(){return socketFd_;}
    void setReady(bool flag);
    bool isReady();
    void setReadPos(const int);
    int getReadPos();
    SharedBuffPtr getBuffPtr();
    int getBuffMaxLength();
    int getBuffCurrentLength();
};
#endif