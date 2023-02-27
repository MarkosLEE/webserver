#include"../head/HttpConnect.h"
HttpConnect::HttpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_):
    TcpConnect(socketFd_, buffMaxLength_, epoll_)
    {}
