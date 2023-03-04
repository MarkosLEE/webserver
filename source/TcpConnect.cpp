#include"../head/TcpConnect.h"
#include<cstdio>
#include<errno.h>
#include<string.h>
TcpConnect::TcpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_):
        socketFd_(socketFd_),
        buffMaxLength_(buffMaxLength_),
        buff_(new char[buffMaxLength_]),
        buffCurentLength_(0),
        buffReadPos_(0),
        epoll_(epoll_),
        iv_(new struct iovec[2])
    {}    

char* TcpConnect::get(){
    return buff_.get();
}

TcpConnect::~TcpConnect(){
    epoll_->del(socketFd_);
    Socket::socketClose(socketFd_);
}

//返回false，代表连接出错，从map中删去(析构函数自动关闭socket)
bool TcpConnect::read(){
    char* buffPtr_=buff_.get();
    while(true){
        if(buffCurentLength_>=buffMaxLength_){
            printf("data overflow!\n");
            return false;
        }
        int tempLength=Socket::socketRead(socketFd_,buffPtr_+buffCurentLength_,buffMaxLength_-buffCurentLength_);
        if(tempLength==0){
            //对端连接关闭
            printf("socket close\n");
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
    while (buffCurentLength_>=0){
        int temp=Socket::socketWrite(socketFd_, buff_.get(),buffCurentLength_);
        if(temp<0){
            // 如果TCP写缓冲没有空间，则等待下一轮EPOLLOUT事件
            if(errno==EAGAIN){
                epoll_->mod(socketFd_,EPOLLOUT);
                return true;
            }
            return false;
        }
        else{
            buffCurentLength_=buffCurentLength_-temp;
        }
    }
    buffCurentLength_=0;
    memset(buff_.get(),0,buffMaxLength_);//发送成功，将缓冲区置0
    setReady(false);
    return true;
}

bool TcpConnect::wirtev(){
    while (buffCurentLength_>=0)
    {
        int temp=Socket::socketWritev(socketFd_, iv_.get(),ivCount_);
        if(temp<0){
            // 如果TCP写缓冲没有空间，则等待下一轮EPOLLOUT事件
            if(errno==EAGAIN){
                epoll_->mod(socketFd_,EPOLLOUT);
                return true;
            }
            printf("errno:%d\n",errno);
            return false;
        }
        else{
            buffCurentLength_=buffCurentLength_-temp;
        }
    }
    buffCurentLength_=0;
    memset(buff_.get(),0,buffMaxLength_);//发送成功，将缓冲区置0
    setReady(false);
    return true;
}

void TcpConnect::setReady(bool flag){
    isReady_=flag;
}

bool TcpConnect::isReady(){
    return isReady_;
}

void TcpConnect::setReadPos(const int pos){
    buffReadPos_=pos;
}

int TcpConnect::getReadPos(){
    return  buffReadPos_;
}

SharedBuffPtr TcpConnect::getBuffPtr(){
    return buff_;
}

int TcpConnect::getBuffMaxLength(){
    return buffMaxLength_;
}

int TcpConnect::getBuffCurrentLength(){
    return buffCurentLength_;
}