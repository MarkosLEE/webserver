#ifndef SOCKET_H
#define SOCKET_H
#include"Type.h"
#include<sys/types.h>//数据类型定义
#include<sys/socket.h>//socket函数及数据结构
#include<netinet/in.h>//提供数据结构sockaddr_in
#include<arpa/inet.h>//提供IP地址转换函数
#include<unistd.h>//提供通用的文件、目录、程序及进程操作的函数
#include<fcntl.h>//设置read,wirte非阻塞
//工具类不可实例化
class Socket
{
private:
    Socket(){}
    ~Socket(){}
public: 
    static int socketInit(){
        //协议族 套接字接口类型 传输协议编号 返回socketfd
        int socketFd=socket(AF_INET,SOCK_STREAM,0);
        return socketFd;
    }
    static int socketBind(const int fd,const string& addr,const int port);//绑定端口号和IP
    static int socketSetPortReuse(const int fd){
        int reuse=1;
        return setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    }//设置端口复用，使得端口可以绑定多个连接
    static int socketListen(const int fd,const int backlog){
        return listen(fd,backlog);
    }//进入监听状态,设置最大等待连接数
    static int socketAccept(const int fd);//建立socket连接
    static int socketConnect(const int fd,const int port,const string addr);//向目的主机发出建立socket连接的请求
    static ssize_t socketWrite(const int fd,const char* msg,const size_t len);//返回-1代表失败
    static ssize_t socketWritev(const int fd,struct iovec* iv,int ivCount);
    static ssize_t socketRead(const int fd,char *buf,size_t len);//返回-1代表失败
    static void socketClose(const int fd){close(fd);}//关闭socket连接
    static bool socketSetNoBlocking(const int fd);//设置socket非阻塞
};



#endif