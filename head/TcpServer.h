#ifndef TCPSERVER_H
#define TCPSERVER_H
class TcpServer
{
private:
    int  port_;//服务器监听的端口
    int  MaxConnection_;//最大连接数
    
public:
    TcpServer(int port_,int MaxConnection_):
    port_(port_),MaxConnection_(MaxConnection_){}
    ~TcpServer();
    bool start();//启动服务器
    
};

#endif