#include "network/SocketServer.h"

SocketServer::SocketServer(int, const char *) {

}

SocketServer::~SocketServer() {

}

void SocketServer::init() {
    info.sin_family = AF_INET;
    info.sin_port = htons(port);
    inet_pton(AF_INET, address, &info.sin_addr.s_addr);

    if(!WSAStartup(MAKEWORD(2, 2), &wsa)){

    }

    socket = ::socket(AF_INET, SOCK_DGRAM, 0);

    bind(socket, (struct sockaddr*) &info, sizeof(info));

    char buf[16];
    inet_ntop(AF_INET, &info.sin_addr, buf, 16);
    PLOGD << "Server started at: " << buf << ":" << ntohs(port);
}
