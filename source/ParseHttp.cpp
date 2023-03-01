#include"../head/ParseHttp.h"
#include<algorithm>
const std::vector<string> ParseHttp::methodName={"GET","POST","HEAD","PUT","DELETE","TRACE","OPTIONS","CONNECT"};

const std::vector<string> ParseHttp::httpVersion={"HTTP/1.1","HTTP/1.0"};

const std::vector<string> ParseHttp::codeDescript={"Bad Request","Forbidden","Not Found","OK"};

const std::vector<string> ParseHttp::errorInfo={
    "Your request has bad syntax or is inherently impossible to satisfy.\n",
    "You do not have permission to get file from this server.\n",
    "The requested file was not found on this server.\n"
};
const std::vector<int> ParseHttp::stateNum={400,403,404,200};
int ParseHttp::requestKind(char* &buff){
    string request;
    while((*buff)!=' '){
        request=request+(*buff);
        buff++;
    }
    buff++;//跳过空格
    auto res=std::find(ParseHttp::methodName.begin(),ParseHttp::methodName.end(),request);
    if(res!=ParseHttp::methodName.end()){
        return std::distance(ParseHttp::methodName.begin(),res);
    }
    else{
        return -1;//请求报文类型错误，断开连接
    }
}
string ParseHttp::requestResource(char* &buff){
    string path;
    while((*buff)!=' '){
        path=path+(*buff);
        buff++;
    }
    buff++;//跳过空格
    return path;
}
int ParseHttp::requestVersion(char* &buff){
    string version;
    while((*buff)!='\r'){
        version=version+(*buff);
        buff++;
    }
    buff++;
    buff++;//跳过换行符，进入下一行
    auto res=std::find(ParseHttp::httpVersion.begin(),ParseHttp::httpVersion.end(),version);
    if(res!=ParseHttp::httpVersion.end()){
        return std::distance(ParseHttp::httpVersion.begin(),res);
    }
    else{
        printf("version:%s error!\n",version.c_str());
        return -1;//版本号错误，断开连接
    }
}
string ParseHttp::requestHost(char* &buff){
    string host;
    while((*buff)!=' '){
        buff++;
    }
    buff++;
    while((*buff)!='\r'){
        host=host+(*buff);
        buff++;
    }
    buff++;
    buff++;//进入下一行
    return host;
} 

bool ParseHttp::isKeepLive(char* &buff){
    string str;
    while((*buff)!=' '){
        buff++;
    }
    buff++;
    while((*buff)!='\r'){
        str=str+(*buff);
        buff++;
    }
    buff++;
    buff++;
    //printf("%s\n",str.c_str());
    if(str=="keep-alive"){
        return true;
    }
    else{
        return false;
    }
}