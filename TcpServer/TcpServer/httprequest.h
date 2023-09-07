#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class HttpRequest{
private:
    std::string url;
    std::string data;
public:
    HttpRequest();
    void setUrl(std::string host);
    void setData(std::string data);
    std::string sendRequest();
};

#endif // HTTPREQUEST_H
