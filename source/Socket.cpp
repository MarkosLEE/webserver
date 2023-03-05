#include "../head/Socket.h"
#include <sys/uio.h>
int Socket::socketBind(const int fd, const string &addr, const int port)
{
    struct sockaddr_in address;
    address.sin_addr.s_addr = inet_addr(addr.c_str()); // 填充地址
    address.sin_family = AF_INET;                      // 使用ip4地址
    address.sin_port = htons(port);                    // 设置端口
    return bind(fd, (struct sockaddr *)&address, sizeof(address));
}
int Socket::socketAccept(const int fd)
{
    struct sockaddr_in socketIn; // 存放提出连接请求服务的主机的信息
    socklen_t len = sizeof(struct sockaddr_in);
    return accept(fd, (struct sockaddr *)&socketIn, &len);
}
int Socket::socketConnect(const int fd, const int port, const string addr)
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;                      // 使用IPv4地址
    address.sin_addr.s_addr = inet_addr(addr.c_str()); // 具体的IP地址
    address.sin_port = htons(1234);                    // 端口
    return connect(fd, (struct sockaddr *)&address, sizeof(address));
}
ssize_t Socket::socketWrite(const int fd, const char *msg, const size_t len)
{
    return write(fd, msg, len);
}
ssize_t Socket::socketWritev(const int fd, struct iovec *iv, int ivCount)
{
    return writev(fd, iv, ivCount);
}

ssize_t Socket::socketRead(const int fd, char *msg, size_t len)
{
    return read(fd, msg, len);
}
bool Socket::socketSetNoBlocking(const int fd)
{
    // 设置socket非阻塞
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1;
}