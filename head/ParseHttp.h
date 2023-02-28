#ifndef PARSEHTTP_H
#define PARSEHTTP_H
#include"Type.h"
#include<vector>
//工具类
class ParseHttp
{
private:
    ParseHttp(/* args */){}
    ~ParseHttp(){}
public:
    // HTTP请求方法，这里只支持GET
    enum method {GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT,ERROR};
    enum HttpCode { BAD_REQUEST=0, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST };
    static const std::vector<string>methodName;
    static const std::vector<string>httpVersion;
    static const std::vector<string>codeDescript;//状态码描述
    static const std::vector<string>errorInfo;//错误状态信息
    static const std::vector<int>stateNum;//状态码
    static int requestKind(char* &buf);//判断请求类型，get\post
    static string requestResource(char* &buf);//获取请求资源路径
    static int requestVersion(char* &buf);//判断http版本，目前只考虑了http1.1,其他暂不考虑
    static bool isKeepLive(char* &buf);//是否保持连接
    static string requestHost(char* &buf);//目标主机的IP和端口号
};
#endif