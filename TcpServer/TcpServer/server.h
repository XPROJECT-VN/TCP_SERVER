#ifndef SERVER_H
#define SERVER_H
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class Server{
private:
    std::string port = "8001";
    WSADATA wsaData;
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
public:
    Server(unsigned short port = 8001);
    /* @brief Start server for listen connect request
     */
    bool start();
    /* @brief Wait util a client connect
     */
    bool waitConnection();
    /* @brief Disconnect to client
     */
    void disconnect();
    /* @brief Get data from client and store into a buffer
     * @param out: pointer to buffer
     * @param size: size of data want to reveive (less than or equal to the buffer size)
     */
    int recvData(char* out, size_t size);
    /* @brief Send data to client
     * @param out: pointer to buffer
     * @param size: size of data want to send (less than or equal to the buffer size)
     */
    int sendData(const char *in, size_t size);
    /* @brief Stop this server
     */
    void stop();
};

#endif // SERVER_H
