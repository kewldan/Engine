#pragma once

#include "Engine.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <plog/Log.h>
#include <string>

class SocketServer {
private:
    WSADATA wsa;
    bool wsaInitialized;
    SOCKET socket;
    std::string address;
    int port;
    char buffer[65536];
    struct sockaddr_in info;
public:
    SocketServer(int, const char*);
    ~SocketServer();

    SocketServer(const SocketServer &) = delete;

    SocketServer &operator=(const SocketServer &) = delete;

    void init();
};
