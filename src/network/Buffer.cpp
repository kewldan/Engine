#include "network/Buffer.h"
#include <cstring>

Engine::Buffer::Buffer(int length) : capacity(length) {
    ASSERT("Buffer must fit the length prefix", length > HEADER_RESERVE);
    if (capacity < HEADER_RESERVE) {
        capacity = HEADER_RESERVE;
    }
    buffer = new char[capacity];
    clear();
}

void Engine::Buffer::clear() {
    index = HEADER_RESERVE;
    finalized = false;
}

char *Engine::Buffer::get() {
    return buffer;
}

int Engine::Buffer::getIndex() const {
    return index;
}

int Engine::Buffer::getCapacity() const {
    return capacity;
}

bool Engine::Buffer::ensureSpace(int length) const {
    ASSERT("Write length must be >= 0", length >= 0);
    if (length < 0 || length > capacity - index) {
        PLOGE << "Buffer overflow: " << length << " bytes requested, " << (capacity - index) << " left";
        return false;
    }
    return true;
}

void Engine::Buffer::writeRaw(const void *v, int length) {
    if (!ensureSpace(length)) {
        return;
    }
    memcpy(buffer + index, v, length);
    index += length;
}

void Engine::Buffer::writeString(const char *v) {
    ASSERT("Input string is nullptr", v != nullptr);
    const int length = (int) strlen(v);
    writeVarInt(length);
    writeArray(v, length);
}

void Engine::Buffer::writeArray(const char *v, int length) {
    ASSERT("Input array is nullptr", v != nullptr);
    writeRaw(v, length);
}

void Engine::Buffer::writeVarInt(int v) {
    writeVarInt(VInt(v));
}

void Engine::Buffer::writeVarInt(Engine::VInt v) {
    if (!ensureSpace(v.getSize())) {
        return;
    }
    index += v.write(buffer + index);
}

void Engine::Buffer::writeLongLong(long long v) {
    writeRaw(&v, sizeof(v));
}

int Engine::Buffer::getSendBufferSize() {
    if (finalized) {
        PLOGE << "getSendBufferSize() called twice without clear(); the packet is already framed";
        return index;
    }
    finalized = true;
    const int payloadLength = index - HEADER_RESERVE;
    VInt dataLength(payloadLength);
    const int headerSize = dataLength.write(buffer);
    memmove(buffer + headerSize, buffer + HEADER_RESERVE, payloadLength);
    index = headerSize + payloadLength;
    return index;
}

void Engine::Buffer::writeInt(int v) {
    writeRaw(&v, sizeof(v));
}

void Engine::Buffer::writeBool(bool v) {
    writeByte(v ? 1 : 0);
}

void Engine::Buffer::writeByte(char v) {
    writeRaw(&v, sizeof(v));
}

void Engine::Buffer::writeLong(long v) {
    writeRaw(&v, sizeof(v));
}

Engine::Buffer::~Buffer() {
    delete[] buffer;
}
