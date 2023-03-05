#include "../head/ParseHttp.h"
#include "../head/HttpConnect.h"
#include <algorithm>
const std::map<string, int> ParseHttp::methodName = {
    {"GET", GET}, {"POST", POST}, {"HEAD", HEAD}, {"PUT", PUT}, {"DELETE", DELETE}, {"TRACE", GET}, {"OPTIONS", OPTIONS}, {"CONNECT", CONNECT}};

const std::map<string, int> ParseHttp::httpVersion = {{"HTTP/1.0", 0}, {"HTTP/1.1", 1}};

const std::vector<string> ParseHttp::Version = {"HTTP/1.0", "HTTP/1.1"};

const std::vector<string> ParseHttp::codeDescript = {"Bad Request", "Forbidden", "Not Found", "OK"};

const std::vector<string> ParseHttp::errorInfo = {
    "Your request has bad syntax or is inherently impossible to satisfy.\n",
    "You do not have permission to get file from this server.\n",
    "The requested file was not found on this server.\n"};
const std::map<string, int> ParseHttp::header = {
    {"Host", Host}, {"User-Agent", User_Agent}, {"Connection", Connection}, {"Accept", Accept}, {"Accept-Language", Accept_Language}, {"Accept-Encoding", Accept_Encoding}, {"Upgrade-Insecure-Requests", Upgrade_Insecure_Requests}};

const std::vector<int> ParseHttp::stateNum = {400, 403, 404, 200};

int ParseHttp::requestKind(HttpPtr &httpPtr)
{
    SharedBuffPtr tempPtr = httpPtr->getBuffPtr();
    string request;
    int currentPos = httpPtr->getReadPos();
    while (tempPtr[currentPos] != ' ')
    {
        request.push_back(tempPtr[currentPos]);
        currentPos++;
    }
    currentPos++; // 跳过空格
    httpPtr->setReadPos(currentPos);
    if (ParseHttp::methodName.find(request) == ParseHttp::methodName.end())
    {
        return -1;
    }
    else
    {
        return ParseHttp::methodName.at(request);
    }
}

string ParseHttp::requestResource(HttpPtr &httpPtr)
{
    SharedBuffPtr tempPtr = httpPtr->getBuffPtr();
    string path;
    int currentPos = httpPtr->getReadPos();
    while (tempPtr[currentPos] != ' ')
    {
        path.push_back(tempPtr[currentPos]);
        currentPos++;
    }
    currentPos++; // 跳过空格
    httpPtr->setReadPos(currentPos);
    return path;
}

int ParseHttp::requestVersion(HttpPtr &httpPtr)
{
    SharedBuffPtr tempPtr = httpPtr->getBuffPtr();
    string version;
    int currentPos = httpPtr->getReadPos();
    while (tempPtr[currentPos] != '\r')
    {
        version.push_back(tempPtr[currentPos]);
        currentPos++;
    }
    currentPos += 2; // 跳过换行符，进入下一行
    httpPtr->setReadPos(currentPos);
    if (ParseHttp::httpVersion.find(version) == ParseHttp::httpVersion.end())
    {
        return -1;
    }
    else
    {
        return ParseHttp::httpVersion.at(version);
    }
}

bool ParseHttp::parseFirstLine(HttpPtr &httpPtr)
{
    int kind = ParseHttp::requestKind(httpPtr);
    httpPtr->setKind(kind);
    if (kind == -1)
    {
        printf("request kind error!\n");
        return false;
    }
    httpPtr->setResource(ParseHttp::requestResource(httpPtr));
    int version = ParseHttp::requestVersion(httpPtr);
    if (version == -1)
    {
        printf("request version error!\n");
        return false;
    }
    httpPtr->setHttpVersion(ParseHttp::Version[version]);
    return true;
}

int ParseHttp::parseOneLine(HttpPtr &httpPtr)
{
    SharedBuffPtr tempPtr = httpPtr->getBuffPtr();
    int currentPos = httpPtr->getReadPos();
    int currentBuffLength = httpPtr->getBuffCurrentLength();
    // 开头是否为\r\n，即是解析请求头部是否结束
    if (tempPtr[currentPos] == '\r' && currentPos + 1 < currentBuffLength && tempPtr[currentPos + 1] == '\n')
    {
        httpPtr->setReadPos(currentPos + 2);
        return 1;
    }
    string head;
    while (currentPos < currentBuffLength && tempPtr[currentPos] != ':')
    {
        head.push_back(tempPtr[currentPos]);
        currentPos++;
    }
    currentPos++; // 跳过冒号
    string content;
    if (currentPos < currentBuffLength && tempPtr[currentPos] == ' ')
    {
        while (tempPtr[currentPos] != '\r')
        {
            content.push_back(tempPtr[currentPos]);
            currentPos++;
        }
        currentPos++; // 跳过\r
        if (tempPtr[currentPos] != '\n')
        {
            return -1;
        }
    }
    if (ParseHttp::header.find(head) == ParseHttp::header.end())
    {
        // 头部字段不存在
        return -1;
    }
    // 重载[]的重载函数不是const函数，所以const对象无法调用
    // 若at不存在时，会抛出out of range异常，但是我们上面已经检查过head存在了
    switch (ParseHttp::header.at(head))
    {
    case Host:
        // 验证是否与服务器IP和监听端口号一致
        break;
    case Connection:
        httpPtr->setKeepAlive(content);
        break;
    default:
        break;
    }
    return 0;
}