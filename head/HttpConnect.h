#ifndef HTTPCONNECT_H
#define HTTPCONNECT_H
#include"TcpConnect.h"
struct Request
{
    string httpVersion_;//http版本
    string resource_;//请求的资源
    bool isKeepAlive_;
    int kind_;//请求报文类型
    //更多属性带扩充
};

class HttpConnect:public TcpConnect
{
private:
    struct Request request_;//请求报文的相关属性
public:
    HttpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_);
    ~HttpConnect();
    void setKeepAlive(const string&);
    bool isKeepAlive();
    void setCount(int count);//设置缓冲区个数
    void setHttpVersion(const string&);//设置版本号
    HttpCode setIv(const string path,const string rootPath);//设置要发送的文件
    void setGetReply(HttpCode);//根填充Get请求的响应报文
    void append(char* &ptr,const string &);//响应报文添加信息
    void setKind(const int kind);//设置请求报文种类
    int getKind();
    void setResource(const string&);
    const string getResource();
    int release();//释放内存映射
};
typedef std::shared_ptr<HttpConnect> HttpPtr;
#endif