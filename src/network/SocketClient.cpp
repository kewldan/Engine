#include "network/SocketClient.h"

Engine::SocketClient::SocketClient() {
    int err = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (err != 0) {
        PLOGE << "Failed to start WSA: " << WSAGetLastError();
    }
}

void Engine::SocketClient::connect(const char *ip, int port) {
    socket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket == INVALID_SOCKET) {
        closesocket(socket);
        WSACleanup();
        PLOGE << "Failed to open socket";
    }

    in_addr ip_to_num{};
    inet_pton(AF_INET, ip, &ip_to_num);

    sockaddr_in servInfo{};
    ZeroMemory(&servInfo, sizeof(servInfo));
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(port);

    int err = ::connect(socket, (sockaddr*)&servInfo, sizeof(servInfo));

    if(!err) connected = true;
}

void Engine::SocketClient::disconnect() {
    shutdown(socket, SD_SEND);
    connected = false;
}

bool Engine::SocketClient::isConnected() const {
    return connected;
}

unsigned long Engine::SocketClient::getAvailable() const {
    unsigned long available = 0;
    if(isConnected()){
        ioctlsocket(socket, FIONREAD, &available);
    }
    return available;
}

void Engine::SocketClient::sendArray(const char *data, int length) {
    if(send(socket, data, length, 0) == -1){
        connected = false;
        PLOGW << "Client disconnected";
    }
}

bool Engine::SocketClient::receiveArray(char *buffer, int length) {
    if(recv(socket, buffer, length, 0) != length){
        connected = false;
        PLOGW << "Client disconnected";
        return false;
    }else{
        return true;
    }
}
