#pragma once

#ifdef _WIN32

#include <winsock2.h>
#include <Ws2tcpip.h>

#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace Engine {

    const int SEGMENT_BITS = 0x7F;
    const int CONTINUE_BIT = 0x80;

    class VInt {
        int value;
    public:
        explicit VInt(int val);

        static VInt *read(SOCKET socket);

        static VInt *read(const char *buffer);

        int write(char *buffer) const;

        int getSize();

        [[nodiscard]] int getValue() const;

        void setValue(int val);
    };
}