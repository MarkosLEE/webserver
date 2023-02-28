#include"../head/WebServer.h"
#include"../head/ParseHttp.h"
#include<assert.h>
WebServer::WebServer(int port_,int maxConnection_,int maxEvents_,const string& name_,const string& address_,int threadNum_):
    TcpServer( port_, maxConnection_, maxEvents_,  name_, address_, threadNum_),
    connects_()
    {}
bool WebServer::start(){
    //完成listenfd对于端口和IP的绑定
    int listenFd=listenConnect_.fd();
    if(listenFd<0){
        printf("listen socket error!\n");
        return false;
    }
    assert(Socket::socketSetPortReuse(listenFd)==0);
    assert(Socket::socketBind(listenFd,address_,port_)==0);
    assert(epollPtr_->append(listenFd,false));
    assert(Socket::socketListen(listenFd,100)==0);
    //启动工作线程池
    if(!threadPoolPtr_->start(threadNum_)){
        printf("threadpool start fail in WebServer start!\n");
        return false;
    }
    printf("start!\n");
    while (true)
    {
        //printf("wait events\n");
        int count=epollPtr_->wait();
        //printf("count num:%d\n",count);
        for(int i=0; i<count; i++){
            int eventFd=epollPtr_->events_[i].data.fd;
            if(eventFd==listenFd){
                //新的连接
                if(connects_.size()>=maxConnection_){
                    printf("socket connect up to max!\n");
                    continue;
                }
                int newFd=Socket::socketAccept(listenFd);
                if(newFd<0){
                    printf("accept new socket fail\t errno:%d",errno);
                    continue;
                }
                //printf("connect!\n");
                epollPtr_->append(newFd,true);
                connects_.insert(std::make_pair(newFd,std::make_unique<HttpConnect>(newFd,buffMaxLength_,epollPtr_)));
                printf("connect num:%ld\n",connects_.size());
            }
            else if(epollPtr_->events_[i].events&( EPOLLRDHUP | EPOLLHUP | EPOLLERR )){
                //发送三种事件：读关闭 读写都关闭 错误事件，至少发送一件，关闭连接
                printf("socket shutdown\n");
                if(epollPtr_->events_[i].events){
                    printf("events error\n");
                }
                connects_.erase(eventFd);
                printf("erase socketfd:%d!\n",eventFd);
            }
            else if(epollPtr_->events_[i].events&EPOLLIN){
                //读事件
                //printf("handleRead\n");
                threadPoolPtr_->run(std::bind(&WebServer::handleRead,this,eventFd));
            }
            else if(epollPtr_->events_[i].events&EPOLLOUT){
                //写事件
                //printf("handleWrite\n");
                threadPoolPtr_->run(std::bind(&WebServer::handleWrite,this,eventFd));
            }
        }
    }
    return true;
}

void WebServer::handleRead(const int socketFd){
    if(!connects_[socketFd]->read()){
        //printf("read data error!\n");
        connects_.erase(socketFd);
        return;
    }
    //printf("%s",connects_[socketFd]->get());
    threadPoolPtr_->run(std::bind(&WebServer::handleParseHttp,this,socketFd));
}
void WebServer::handleParseHttp(const int socketFd){
    char *buff=connects_[socketFd]->get();
    
    int kind=ParseHttp::requestKind(buff);
    if(kind==-1){
        connects_.erase(socketFd);
        return;
    }
    else if(kind==ParseHttp::GET){
        //获取资源地址
        string path=ParseHttp::requestResource(buff);
        //获取http版本
        int version=ParseHttp::requestVersion(buff);
        if(version==-1){
            //版本号错误，断开连接
            connects_.erase(socketFd);
            return;
        }
        else{
            connects_[socketFd]->setHttpVersion(ParseHttp::httpVersion[version]);
        }
        if(ParseHttp::requestHost(buff)!=address_+":"+std::to_string(port_)){
            //与服务器IP端口不匹配（服务器IP使用的是0.0.0.0）所以一定会匹配成功
            //connects_.erase(socketFd);
        }
        connects_[socketFd]->setKeepAlive(ParseHttp::isKeepLive(buff));
        //设置发送数据
        ParseHttp::HttpCode httpCode=connects_[socketFd]->setIv(path,rootPath);
        connects_[socketFd]->setReply(httpCode);
    }
    //printf("%s",connects_[socketFd]->get());
    //最后要注册监听写事件
    epollPtr_->mod(socketFd,EPOLLOUT);
    //threadPoolPtr_->run(std::bind(&WebServer::handleWrite,this,socketFd));
}
void WebServer::handleWrite(const int socketFd){

    if(!connects_[socketFd]->wirtev()){
        //发送失败，断开连接
        connects_.erase(socketFd);
        return;
    }
    if(connects_[socketFd]->isReady()){
        //上次发送缓冲区满了,等待下次写事件
        return;
    }
    if(!connects_[socketFd]->isKeepAlive()){
        //断开连接
        connects_.erase(socketFd);
        return;
    }
    //发送成功，重新注册可读事件
    epollPtr_->mod(socketFd,EPOLLIN);

}
void WebServer::setRootPath(const string&path){
    rootPath=path;
    return;
}