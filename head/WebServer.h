#ifndef WEBSERVER_H
#define WEBSERVER_H
#include"TcpServer.h"
#include"HttpConnect.h"
class WebServer:public TcpServer
{
private:
    std::map<int,std::unique_ptr<HttpConnect>>connects_;//fd作为键
public:
    WebServer(int port_,int maxConnection_,int maxEvents_,const string& name_,const string& address_,int threadNum_);
    ~WebServer(){}
    virtual bool start() override;//启动服务器
    virtual void handleRead(const int fd) override;//处理读事件
    virtual void handleWrite(const int fd) override;//处理写事件
    virtual void handleParseHttp(const int fd) override;//解析http报文
};
#endif