#pragma once

#include <cstddef>

#ifdef _WIN32

#include <winsock2.h>
#include <Ws2tcpip.h>

#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int SOCKET;
#endif

namespace Engine {

    constexpr int SEGMENT_BITS = 0x7F;
    constexpr int CONTINUE_BIT = 0x80;

    // LEB128-style variable-length 32-bit integer (max 5 bytes on the wire).
    class VInt {
        int value;
    public:
        static constexpr int MAX_SIZE = 5;

        explicit VInt(int val);

        // The read() overloads return a heap-allocated VInt (delete it) or nullptr on
        // a closed socket / malformed input.
        static VInt *read(SOCKET socket);

        // Reads from `buffer`, which must hold at least MAX_SIZE bytes (or the whole encoded value).
        static VInt *read(const char *buffer);

        // Bounded variant: never reads past `buffer + length`.
        static VInt *read(const char *buffer, std::size_t length);

        // Writes up to MAX_SIZE bytes; returns how many were written.
        int write(char *buffer) const;

        [[nodiscard]] int getSize() const;

        [[nodiscard]] int getValue() const;

        void setValue(int val);
    };
}
