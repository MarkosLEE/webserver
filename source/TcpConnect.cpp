#include"../head/TcpConnect.h"
#include<cstdio>
#include<errno.h>
TcpConnect::TcpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_):
        socketFd_(socketFd_),
        buffMaxLength_(buffMaxLength_),
        buff_(new char[buffMaxLength_]),
        buffCurentLength_(0),
        epoll_(epoll_)
    {
        buffPtr_=buff_.get();
    }
char* TcpConnect::get(){
    return buff_.get();
}
//返回false，代表连接出错，从map中删去(析构函数自动关闭socket)
bool TcpConnect::read(){
    while(true){
        if(buffCurentLength_>=buffMaxLength_){
            printf("data overflow!\n");
            return false;
        }
        int tempLength=Socket::socketRead(socketFd_,buffPtr_+buffCurentLength_,buffMaxLength_-buffCurentLength_);
        if(tempLength==0){
            //对端连接关闭
            return false;
        }
        else if(tempLength>0){
            buffCurentLength_=buffCurentLength_+tempLength;
        }
        else{
            if(errno==EAGAIN || errno==EWOULDBLOCK){
                //连接未出错，缓冲区没有数据可读   
                return true;
            }
            //其他错误
            printf(" socket read fail!\n");
            return false;
        }
    }
}
//返回false，代表连接出错，从map中删去(析构函数自动关闭socket)
//返回true,并不代表写入成功，可能需要重新调用wirte(通过判断currentLength_)
bool TcpConnect::wirte(){
    if(Socket::socketWrite(socketFd_,buffPtr_,buffCurentLength_)<0){
        if(errno==EAGAIN){
            epoll_->mod(socketFd_,EPOLLOUT);
            return true;
        }
        //写失败
        printf("fd %d wirte error!\n",socketFd_);
        return false;
    }
    //假设一次性全部写成功
    else{
        buffCurentLength_=0;
        return true;
    }
}
bool TcpConnect::wirtev(){
    if(Socket::socketWritev(socketFd_, iv.get(),ivCount)<0){
        // 如果TCP写缓冲没有空间，则等待下一轮EPOLLOUT事件
        if(errno==EAGAIN){
            epoll_->mod(socketFd_,EPOLLOUT);
        }
        return false;
    }
    return true;
}