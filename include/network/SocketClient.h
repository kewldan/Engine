#pragma once

#include "plog/Log.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace Engine {
    class SocketClient {
    private:
        bool connected{};
        WSADATA wsData{};
        void sendArray(const char* data, int length);
        bool receiveArray(char* buffer, int length);
    public:
        SOCKET socket{};

        SocketClient();
        void connect(const char* ip, int port);
        void disconnect();
        bool isConnected() const;
        unsigned long getAvailable() const;
    };
}
