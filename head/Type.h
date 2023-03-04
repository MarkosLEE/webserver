#ifndef TYPE_H
#define TYPE_H
#include<string>
using std::string;
// HTTP请求方法，这里只支持GET
enum HttpMethod {GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT,ERROR};
enum HttpCode { BAD_REQUEST=0, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST };
enum HttpHeader{Host=0,User_Agent,Connection,Accept,Accept_Language,Accept_Encoding,Upgrade_Insecure_Requests};
#endif