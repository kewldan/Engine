#include "network/SocketServer.h"

SocketServer::SocketServer(int port, const char *address) : wsa{}, wsaInitialized(false), socket(INVALID_SOCKET),
                                                            address(address ? address : ""), port(port), buffer{},
                                                            info{} {
    ASSERT("Address is nullptr", address != nullptr);
}

SocketServer::~SocketServer() {
    if (socket != INVALID_SOCKET) {
        closesocket(socket);
    }
    if (wsaInitialized) {
        WSACleanup();
    }
}

void SocketServer::init() {
    if (socket != INVALID_SOCKET) {
        PLOGW << "Server already initialised";
        return;
    }

    if (!wsaInitialized) {
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            PLOGE << "WSAStartup failed";
            return;
        }
        wsaInitialized = true;
    }

    info.sin_family = AF_INET;
    info.sin_port = htons((u_short) port);
    if (inet_pton(AF_INET, address.c_str(), &info.sin_addr.s_addr) != 1) {
        PLOGE << "Invalid server address: " << address;
        return;
    }

    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket == INVALID_SOCKET) {
        PLOGE << "Socket creation failed: " << WSAGetLastError();
        return;
    }

    if (bind(socket, (struct sockaddr *) &info, sizeof(info)) == SOCKET_ERROR) {
        PLOGE << "Bind failed: " << WSAGetLastError();
        closesocket(socket);
        socket = INVALID_SOCKET;
        return;
    }

    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &info.sin_addr, buf, sizeof(buf));
    PLOGD << "Server started at: " << buf << ":" << port;
}
