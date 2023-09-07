#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

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
                HttpRequest request;
                request.setUrl("http://ip.jsontest.com");
                std::string response = request.sendRequest();
                std::cout << response << std::endl;
            }
            else{
                sv.disconnect();
            }
        }
    }
    // stop server
    sv.stop();
    return 0;
}
