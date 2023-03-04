#ifndef WEBSERVER_H
#define WEBSERVER_H
#include"TcpServer.h"
#include"HttpConnect.h"
#include"Type.h"
class WebServer:public TcpServer
{
private:
    std::map<int,HttpPtr>connects_;//fd作为键，非线程安全,写事件全部上锁
    size_t httpConnectCount_;//当前连接的数量
    string rootPath_;//资源根目录
public:
    WebServer(int port_,int maxConnection_,int maxEvents_,const string& name_,const string& address_,int threadNum_);
    ~WebServer(){}
    virtual bool start() override;//启动服务器
    virtual void handleRead(const int fd) override;//处理读事件
    virtual void handleWrite(const int fd) override;//处理写事件
    virtual void handleParseHttp(const int fd) override;//解析http报文
    void setRootPath(const string&);//设置根目录
    void append(int socketFd);//增加连接
    void del(int socketFd);//删除连接
    HttpPtr getHttpConnect(int socketFd);
};
#endif