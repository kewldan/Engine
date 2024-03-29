#include "network/LEB128.h"

Engine::VInt *Engine::VInt::read(SOCKET socket) {
    auto val = new VInt(0);
    int position = 0;
    char currentByte;

    while (true) {
        if (recv(socket, &currentByte, 1, 0) == 1) {
            val->value |= (currentByte & SEGMENT_BITS) << position;

            if ((currentByte & CONTINUE_BIT) == 0) break;

            position += 7;
        } else {
            return nullptr;
        }
    }

    return val;
}

Engine::VInt *Engine::VInt::read(const char *buffer) {
    auto val = new VInt(0);
    int position = 0;
    char currentByte;
    int i = 0;

    while (true) {
        currentByte = buffer[i];
        val->value |= (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0) break;

        position += 7;
        i++;
    }

    return val;
}

int Engine::VInt::write(char *buffer) const {
    int count = 0;
    int val = this->value;

    do {
        unsigned char byte = val & 0x7f;
        val >>= 7;

        if (val != 0)
            byte |= 0x80;  // mark this byte to show that more bytes will follow

        buffer[count] = byte;
        count++;
    } while (val != 0);

    return count;
}

int Engine::VInt::getSize() {
    int size = 0;
    int val = this->value;
    do {
        val >>= 7;
        ++size;
    } while (val != 0);
    return size;
}

int Engine::VInt::getValue() const {
    return value;
}

void Engine::VInt::setValue(int val) {
    value = val;
}

Engine::VInt::VInt(int val) {
    value = val;
}
