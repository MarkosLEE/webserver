#include"../head/WebServer.h"
int main(){
    WebServer s1(8080,100,100,"testServer","0.0.0.0",10);
    s1.setRootPath("/home/lsj/lsj/webserver/resource");
    if(!s1.start()){
        printf("start tcpserver error!\n");
    }
}