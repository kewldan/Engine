#pragma once

#include <plog/Log.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Engine.h"

namespace Engine {

    class SocketServer {
    private:
        WSADATA wsData{};
        struct addrinfo hints{};
        struct addrinfo *result{};
        SOCKET ListenSocket;
    public:
        explicit SocketServer(const char *port);

        SOCKET accept() const;
    };
}
