#include "network/Buffer.h"

Engine::Buffer::Buffer(int length) {
    buffer = new char[length];
    clear();
}

void Engine::Buffer::clear() {
    index = 5;
}

char *Engine::Buffer::get() {
    return buffer;
}

int Engine::Buffer::getIndex() const {
    return index;
}

void Engine::Buffer::writeString(const char *v) {
    ASSERT("Input string is nullptr", v != nullptr);
    writeVarInt((int) strlen(v));
    writeArray(v, (int) strlen(v));
}

void Engine::Buffer::writeArray(const char *v, int length) {
    ASSERT("Input array is nullptr", v != nullptr);
    memcpy(buffer + index, v, length);
    index += length;
}

void Engine::Buffer::writeVarInt(int v) {
    writeVarInt(VInt(v));
}

void Engine::Buffer::writeVarInt(Engine::VInt v) {
    index += v.write(buffer + index);
}

void Engine::Buffer::writeLongLong(long long v) {
    memcpy(buffer + index, &v, sizeof(v));
    index += sizeof(v);
}

int Engine::Buffer::getSendBufferSize() {
    VInt dataLength(index - 5);
    index -= 5;
    index += dataLength.write(buffer);
    memcpy(buffer + dataLength.getSize(), buffer + 5, dataLength.getValue());
    return index;
}

void Engine::Buffer::writeInt(int v) {
    memcpy(buffer + index, &v, sizeof(v));
    index += sizeof(v);
}

void Engine::Buffer::writeBool(bool v) {
    buffer[index] = v ? 1 : 0;
    index++;
}

void Engine::Buffer::writeByte(char v) {
    buffer[index] = v;
    index++;
}

void Engine::Buffer::writeLong(long v) {
    memcpy(buffer + index, &v, sizeof(v));
    index += sizeof(v);
}
