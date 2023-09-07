#include "server.h"
#include <iostream>

using namespace std;


Server::Server(unsigned short port){
    this->port = std::to_string(port);
}

bool Server::start(){
    // Initialize Winsock
    cout << "WSAStartup init ..." << endl;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed with error: " << iResult << "\n" << endl;
        return false;
    }
    cout << "WSAStartup initialized !" << endl;

    cout << "Get address information ..." << endl;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;


    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if ( iResult != 0 ) {
        cout << "getaddrinfo failed with error: " << iResult << "\n" << endl;
        WSACleanup();
        return false;
    }
    cout << "Get address information succeed !" << endl;


    // Create a SOCKET for the server to listen for client connections.
    cout << "Create a SOCKET for the server to listen for client connections..." << endl;
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        cout << "socket failed with error: " << WSAGetLastError() << "\n" << endl;;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    cout << "Create a SOCKET for the server to listen for client connections succeed !" << endl;


    // Setup the TCP listening socket
    cout << "Setup the TCP listening socket..." << endl;
    iResult = bind( listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        cout << "bind failed with error: " << WSAGetLastError() << "\n" << endl;;
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return false;
    }
    freeaddrinfo(result);

    // Start listen
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        cout << "listen failed with error: " << WSAGetLastError() << "\n" << endl;;
        closesocket(listenSocket);
        WSACleanup();
        return false;
    }
    cout << "Setup the TCP listening socket succeed !" << endl;
    return true;
}

bool Server::waitConnection(){
    cout << "Wait for client connect ..." << endl;
    if(listenSocket == INVALID_SOCKET){
        return false;
    }

    // Accept a client socket
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        cout << "accept failed with error: " << WSAGetLastError() << endl;;
        return false;
    }
    cout << "A client is accepted !" << endl;;
    return true;
}

void Server::disconnect(){
    if(clientSocket != INVALID_SOCKET){
        shutdown(clientSocket, SD_SEND);
    }
    clientSocket = INVALID_SOCKET;
}

int Server::recvData(char* out, size_t size){
    if(listenSocket == INVALID_SOCKET){
        return -1;
    }
    if(clientSocket == INVALID_SOCKET){
        return -1;
    }
    // Receive until the peer shuts down the connection
    int iResult = recv(clientSocket, out, size, 0);
    if (iResult < 1) {
        cout << "No data received" << endl;
        return -1;
    }
    cout << "Bytes received: " << iResult << endl;
    return iResult;
}

int Server::sendData(const char *in, size_t size){
    if(listenSocket == INVALID_SOCKET){
        return -1;
    }
    if(clientSocket == INVALID_SOCKET){
        return -1;
    }
    int iSendResult = send(clientSocket, in, size, 0 );
    if (iSendResult == SOCKET_ERROR) {
        cout << "Send failed with error: " << WSAGetLastError() << endl;
        cout << "Bytes sent: 0" << endl;
    }
    else{
        cout << "Send data succeed" << endl;
        cout << "Bytes sent: " << iSendResult << endl;
    }
    return iSendResult;
}

void Server::stop(){
    if(listenSocket != INVALID_SOCKET){
        closesocket(listenSocket);
    }
    WSACleanup();
    listenSocket = INVALID_SOCKET;
}
