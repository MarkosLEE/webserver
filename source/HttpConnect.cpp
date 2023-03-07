#include "../include/HttpConnect.h"
#include "../include/ParseHttp.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
HttpConnect::HttpConnect(const int socketFd_, int buffMaxLength_, std::shared_ptr<Epoll> epoll_) : TcpConnect(socketFd_, buffMaxLength_, epoll_)
{
}

HttpConnect::~HttpConnect()
{
    release();
}

void HttpConnect::setCount(int count)
{
    ivCount_ = count;
    return;
}

HttpCode HttpConnect::setIv(const string path, const string rootPath)
{
    setCount(1);
    string filePath = rootPath + path;
    if (path == "/")
    {
        // 默认文件
        filePath = filePath + "index.html";
    }
    struct stat fileStat; // 目标文件的状态。可以判断文件是否存在、是否为目录、是否可读，并获取文件大小等信息
    if (stat(filePath.c_str(), &fileStat) < 0)
    {
        // 没有该资源
        // printf("%s\n",filePath.c_str());
        // printf("errno:%d\n",errno);
        return NO_RESOURCE;
    }
    // 判断访问权限
    if (!(fileStat.st_mode & S_IROTH))
    {
        return FORBIDDEN_REQUEST;
    }
    // 判断是否是目录
    if (S_ISDIR(fileStat.st_mode))
    {
        return BAD_REQUEST;
    }
    // 以只读方式打开文件
    int fd = open(filePath.c_str(), O_RDONLY);
    // 创建内存映射
    if (fd < 0)
    {
        printf("fd return error!\n");
        return BAD_REQUEST;
    }
    char *fileAddress = (char *)mmap(0, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    setCount(2);
    iv_[1].iov_base = fileAddress;
    iv_[1].iov_len = fileStat.st_size;
    return FILE_REQUEST;
}

void HttpConnect::setGetReply(HttpCode httpCode)
{
    // 先清空缓冲区
    memset(buff_.get(), 0, buffMaxLength_);
    buffCurentLength_ = 0;
    char *buffPtr = buff_.get();
    // 填充版本号
    append(buffPtr, request_.httpVersion_ + ' ');
    // 填充状态码和状态码描述
    string stateLine;
    stateLine = stateLine + std::to_string(ParseHttp::stateNum[httpCode]) + ' ' + ParseHttp::codeDescript[httpCode] + "\r\n";
    append(buffPtr, stateLine);
    // 添加必要的响应头部
    // 添加content-length字段
    string contentLength = "Content-Length: ";
    if (httpCode == FILE_REQUEST)
    {
        contentLength = contentLength + std::to_string(iv_[1].iov_len) + "\r\n";
    }
    else
    {
        contentLength = contentLength + std::to_string(ParseHttp::errorInfo[httpCode].size()) + "\r\n";
    }
    append(buffPtr, contentLength);
    // 添加content-type字段
    string contentType = "Content-Type: text/html\r\n";
    append(buffPtr, contentType);
    // 添加connect字段
    string connection = "Connection: ";
    if (request_.isKeepAlive_)
    {
        connection = connection + "keep-alive\r\n";
    }
    else
    {
        connection = connection + "close\r\n";
    }
    append(buffPtr, connection);
    // 最后添加回车符和换行符
    append(buffPtr, "\r\n");
    iv_[0].iov_base = buff_.get();
    iv_[0].iov_len = buffCurentLength_;
    setReady(true);
}

void HttpConnect::setKeepAlive(const string &str)
{
    if (str.compare("keep-alive") == 0)
    {
        request_.isKeepAlive_ = true;
    }
    else
    {
        request_.isKeepAlive_ = false;
    }
}

bool HttpConnect::isKeepAlive()
{
    return request_.isKeepAlive_;
}

void HttpConnect::setHttpVersion(const string &str)
{
    // printf("version:%s\n",str.c_str());
    request_.httpVersion_ = str;
}

void HttpConnect::append(char *&ptr, const string &str)
{
    strcpy(ptr + buffCurentLength_, str.c_str());
    buffCurentLength_ = buffCurentLength_ + str.size();
}

void HttpConnect::setKind(const int kind)
{
    request_.kind_ = kind;
}

void HttpConnect::setResource(const string &path)
{
    // printf("path:%s\n",path.c_str());
    request_.resource_ = path;
}

const string HttpConnect::getResource()
{
    return request_.resource_;
}

int HttpConnect::getKind()
{
    return request_.kind_;
}

int HttpConnect::release()
{
    if (ivCount_ > 1)
    {
        return munmap(iv_[1].iov_base, iv_[1].iov_len);
    }
    return 0;
}