#include "httprequest.h"
#include <vector>
#include <iostream>

using namespace std;

HttpRequest::HttpRequest(){

}

void HttpRequest::setUrl(std::string host){
    this->url = host;
}

void HttpRequest::setData(std::string data){
    this->data = data;
}

std::string HttpRequest::sendRequest(){
    WSADATA wsaData;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return "WSAStartup failed with error:" + std::to_string(iResult);
    }

    std::string hostname = url;
    if(hostname.find_first_of("https://")==0){
        hostname = hostname.substr(7);
    }
    else if(hostname.find_first_of("http://")==0){
        hostname = hostname.substr(6);
    }
    int p = hostname.find_first_of("/");
    hostname = hostname.substr(0, p);

    struct hostent* ht = gethostbyname(hostname.c_str());
    if(ht == nullptr){
        int error = WSAGetLastError();
        WSACleanup();
        return "Could not detect host info with code : " + std::to_string(error);
    }
    SOCKADDR_IN sockAddr;
    sockAddr.sin_port = htons(80);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = *((unsigned long*)ht->h_addr);
    SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(connect(sock, (SOCKADDR*)&sockAddr, sizeof (sockAddr)) != 0){
        WSACleanup();
        return "Could not connect";
    }
    std::string requestStr = "GET / HTTP/1.1\r\nHost: " + hostname + "\r\nConnection: close\r\n\r\n";

    int bytes = send(sock, requestStr.c_str(), requestStr.length(),0);
    if(bytes < 0){
        closesocket(sock);
        WSACleanup();
        return "Could not send data";
    }

    char buffer[1024];
    std::vector<char> data;
    do{
        bytes = recv(sock, buffer, 1024,0);
        for(int i=0;i<bytes;i++){
            data.push_back(buffer[i]);
        }
    }while(bytes > 0);
    data.push_back(0);

    std::string sdata = std::string(data.data(), data.data() + data.size());

    closesocket(sock);
    WSACleanup();
    return sdata;
}
