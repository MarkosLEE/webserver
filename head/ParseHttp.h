#ifndef PARSEHTTP_H
#define PARSEHTTP_H
#include "Type.h"
#include "HttpConnect.h"
#include <vector>
#include <map>
// 工具类
class ParseHttp
{

private:
    ParseHttp() {}
    ~ParseHttp() {}

public:
    static const std::map<string, int> methodName;
    static const std::map<string, int> httpVersion;
    static const std::vector<string> Version;
    static const std::vector<string> codeDescript; // 状态码描述
    static const std::vector<string> errorInfo;    // 错误状态信息
    static const std::vector<int> stateNum;        // 状态码
    static const std::map<string, int> header;     // 头部字段
    static int requestKind(HttpPtr &);             // 判断请求类型，get\post
    static string requestResource(HttpPtr &);      // 获取请求资源路径
    static int requestVersion(HttpPtr &);          // 判断http版本，目前只考虑了http1.1,其他暂不考虑
    static bool parseFirstLine(HttpPtr &);         // 解析Http请求报文请求行,返回值为指向下一行开头的指针
    static int parseOneLine(HttpPtr &);            // 解析http请求报文请求头部,返回-1代表错误,0代表成功，1代表解析完毕
};
#endif