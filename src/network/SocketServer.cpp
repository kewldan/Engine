#include "network/SocketServer.h"

Engine::SocketServer::SocketServer(const char *port) {
    ASSERT("Port is nullptr", port != nullptr);
    ASSERT("Port length is 0", strlen(port) > 0);
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        PLOGF << "Failed to open socket";
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(nullptr, port, &hints, &result) != 0) {
        PLOGF << "getaddrinfo failed";
        WSACleanup();
        return;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        PLOGF << "socket failed with error: " << WSAGetLastError();
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    if (bind(ListenSocket, result->ai_addr, (int) result->ai_addrlen) == SOCKET_ERROR) {
        PLOGF << "bind failed with error: " << WSAGetLastError();
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        PLOGF << "listen failed with error: " << WSAGetLastError();
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
}

SOCKET Engine::SocketServer::accept() const {
    SOCKET clientSocket = ::accept(ListenSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        PLOGE << "accept failed with error: " << WSAGetLastError();
        return INVALID_SOCKET;
    }
    unsigned long d;
    ioctlsocket(clientSocket, FIONREAD, &d);
    return clientSocket;
}