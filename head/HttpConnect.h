#ifndef HTTPCONNECT_H
#define HTTPCONNECT_H
#include"TcpConnect.h"
#include"ParseHttp.h"
class HttpConnect:public TcpConnect
{
private:
    string httpVersion_;//http版本
    bool isKeepAlive_;
public:
    HttpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_);
    ~HttpConnect(){};
    void setKeepAlive(bool flag);
    bool isKeepAlive();
    void setCount(int count);//设置缓冲区个数
    void setHttpVersion(const string&);//设置版本号
    ParseHttp::HttpCode setIv(const string path,const string rootPath);//设置要发送的文件
    void setReply(ParseHttp::HttpCode);//根据状态码填充响应报文
    void append(char* &ptr,const string &);//响应报文添加信息
};
#endif