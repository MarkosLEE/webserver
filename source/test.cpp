#include "../include/WebServer.h"
int main()
{
    WebServer s1(8080, 1000, 10000, "testServer", "0.0.0.0", 100);
    s1.setRootPath("/home/lsj/lsj/webserver/resource");
    if (!s1.start())
    {
        printf("start tcpserver error!\n");
    }
}