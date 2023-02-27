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
        threadPoolPtr_(std::make_unique<ThreadPool>(name_))
        {}
TcpServer::~TcpServer(){}
