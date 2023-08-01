#pragma once

#include "Engine.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <plog/Log.h>

class SocketServer {
private:
    WSADATA wsa;
    SOCKET socket;
    char* address;
    int port;
    char buffer[65536];
    struct sockaddr_in info;
public:
    SocketServer(int, const char*);
    ~SocketServer();

    void init();
};
