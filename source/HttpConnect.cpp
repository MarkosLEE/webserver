#include"../head/HttpConnect.h"
#include"../head/ParseHttp.h"
#include<sys/stat.h>
#include<sys/mman.h>
#include<string.h>
#include<errno.h>
HttpConnect::HttpConnect(const int socketFd_,int buffMaxLength_,std::shared_ptr<Epoll> epoll_):
    TcpConnect(socketFd_, buffMaxLength_, epoll_)
    {}
void HttpConnect::setCount(int count){
    ivCount_=count;
    return;
}

ParseHttp::HttpCode HttpConnect::setIv(const string path,const string rootPath){
    setCount(1);
    string filePath=rootPath+path;
    if(path=="/"){
        //默认文件
        filePath=filePath+"index.html";
    }
    struct stat fileStat;// 目标文件的状态。可以判断文件是否存在、是否为目录、是否可读，并获取文件大小等信息
    if ( stat( filePath.c_str(), &fileStat ) < 0 ) {
        //没有该资源
        //printf("%s\n",filePath.c_str());
        //printf("errno:%d\n",errno);
        return ParseHttp::NO_RESOURCE;
    }
    // 判断访问权限
    if ( ! ( fileStat.st_mode & S_IROTH ) ) {
        return ParseHttp::FORBIDDEN_REQUEST;
    }
    // 判断是否是目录
    if ( S_ISDIR( fileStat.st_mode ) ) {
        return ParseHttp::BAD_REQUEST;
    }
    // 以只读方式打开文件
    int fd = open( filePath.c_str(), O_RDONLY );
    // 创建内存映射
    char* fileAddress = ( char* )mmap( 0, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0 );
    close( fd );
    setCount(2);
    iv_[1].iov_base=fileAddress;
    iv_[1].iov_len=fileStat.st_size;
    return ParseHttp::FILE_REQUEST;
}

void HttpConnect::setReply(ParseHttp::HttpCode httpCode){
    //先清空缓冲区
    memset(buff_.get(),0,buffMaxLength_);
    buffCurentLength_=0;
    char* buffPtr=buff_.get();
    //填充版本号
    append(buffPtr,httpVersion_+' ');
    //填充状态码和状态码描述
    string  stateLine;
    stateLine=stateLine+std::to_string(ParseHttp::stateNum[httpCode])+' '+ParseHttp::codeDescript[httpCode]+"\r\n";
    append(buffPtr,stateLine);
    //添加必要的响应头部
    //添加content-length字段
    string contentLength="Content-Length: ";
    if(httpCode==ParseHttp::FILE_REQUEST){
        contentLength=contentLength+std::to_string(iv_[1].iov_len)+"\r\n";
    }
    else{
        contentLength=contentLength+std::to_string(ParseHttp::errorInfo[httpCode].size())+"\r\n";
    }
    append(buffPtr,contentLength);
    //添加content-type字段
    string contentType="Content-Type: text/html\r\n";
    append(buffPtr,contentType);
    //添加connect字段
    string connection="Connection: ";
    if(isKeepAlive_){
        connection=connection+"keep-alive\r\n";
    }
    else{
        connection=connection+"close\r\n";
    }
    append(buffPtr,connection);
    //最后添加回车符和换行符
    append(buffPtr,"\r\n");
    iv_[0].iov_base=buff_.get();
    iv_[0].iov_len=buffCurentLength_;
    setReady(true);
}

void HttpConnect::setKeepAlive(bool flag){
    isKeepAlive_=flag;
}

bool HttpConnect::isKeepAlive(){
    return isKeepAlive_;
}

void HttpConnect::setHttpVersion(const string&str){
    httpVersion_=str;
}

void HttpConnect::append(char* &ptr,const string&str){
    strcpy(ptr+buffCurentLength_,str.c_str());
    buffCurentLength_=buffCurentLength_+str.size();
}