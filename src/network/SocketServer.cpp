#include "network/SocketServer.h"

SocketServer::SocketServer(int port, const char *address) : wsa{}, socket(INVALID_SOCKET), address(address),
                                                            port(port), buffer{}, info{} {
}

SocketServer::~SocketServer() {
    if (socket != INVALID_SOCKET) {
        closesocket(socket);
        WSACleanup();
    }
}

void SocketServer::init() {
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        PLOGE << "WSAStartup failed";
        return;
    }

    info.sin_family = AF_INET;
    info.sin_port = htons((u_short) port);
    if (inet_pton(AF_INET, address, &info.sin_addr.s_addr) != 1) {
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
        return;
    }

    char buf[16];
    inet_ntop(AF_INET, &info.sin_addr, buf, 16);
    PLOGD << "Server started at: " << buf << ":" << port;
}
