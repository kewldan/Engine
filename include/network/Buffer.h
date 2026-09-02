#pragma once

#include <plog/Log.h>
#include "LEB128.h"
#include "Engine.h"

namespace Engine {
    // Packet builder: the first 5 bytes are reserved for the LEB128 length prefix that
    // getSendBufferSize() writes in front of the payload.
    //
    // Fixed-width writers (writeInt/writeLong/writeLongLong) emit host byte order (little-endian on
    // x86/ARM) and `long` is 4 bytes on Windows but 8 on Linux/macOS, so the wire format is only
    // stable between peers built for the same platform.
    class Buffer {
    private:
        static constexpr int HEADER_RESERVE = 5; // max size of a LEB128-encoded 32-bit length

        char *buffer;
        int capacity;
        int index{};
        bool finalized{};

        // Returns false (and logs) when `length` more bytes would overflow the buffer.
        bool ensureSpace(int length) const;

        void writeRaw(const void *v, int length);

    public:
        explicit Buffer(int length);

        ~Buffer();

        Buffer(const Buffer &) = delete;

        Buffer &operator=(const Buffer &) = delete;

        void clear();

        char *get();

        [[nodiscard]] int getIndex() const;

        [[nodiscard]] int getCapacity() const;

        // Prepends the payload length and returns the total number of bytes to send.
        // Call once per packet, after all writes; clear() before reusing the buffer.
        int getSendBufferSize();

        void writeString(const char *v);

        void writeArray(const char *v, int length);

        void writeVarInt(int v);

        void writeVarInt(VInt v);

        void writeLong(long v);

        void writeLongLong(long long v);

        void writeInt(int v);

        void writeBool(bool v);

        void writeByte(char v);
    };
}
