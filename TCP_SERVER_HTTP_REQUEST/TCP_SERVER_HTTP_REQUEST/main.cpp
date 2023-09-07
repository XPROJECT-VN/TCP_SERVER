#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#include "server.h"
#include "httprequest.h"



#define RECV_BUFFER_SIZE        2048
#define DEFAULT_LISTEN_PORT     8001
using namespace std;


int main(void){
    // create a server with a port
    Server sv(DEFAULT_LISTEN_PORT);

    // start server and listen client connect request
    if(!sv.start()){
        sv.stop();
    }


    while(true){
        // wait a connection from client
        if(sv.waitConnection()){

            // init a buffer for recv data from client
            char buffer[RECV_BUFFER_SIZE];
            memset(buffer,0, sizeof (buffer));

            // recv data
            int bytes = sv.recvData(buffer, sizeof (buffer));

            // if has data
            if(bytes >0){
                // send a feedback to client.
                string fb = "X=-00.9595;Y=-00.9189;T=+29.08;\n";
                sv.sendData(fb.c_str(), fb.length());
                // and close connection
                sv.disconnect();

                // send a request to host and get data response.
                cout << "Send a http request to http://ip.jsontest.com for get ip address..." << endl;
                HttpRequest request;
                request.setUrl("http://ip.jsontest.com");
                std::string response = request.sendRequest();
                cout << "http://ip.jsontest.com feedback :" << endl;
                std::cout << response << std::endl;
                
            }
            else{
                sv.disconnect();
            }
            std::cout << "-----------------------------------------------------------------------" << std::endl;
        }
    }
    // stop server
    sv.stop();
    return 0;
}
