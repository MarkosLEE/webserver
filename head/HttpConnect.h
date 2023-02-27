#ifndef HTTPCONNECT_H
#define HTTPCONNECT_H
#include"TcpConnect.h"
class HttpConnect:public TcpConnect
{
private:
    
public:
    HttpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_);
    ~HttpConnect(){};
};
#endif