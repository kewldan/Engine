#include "network/LEB128.h"
#include <cstdint>

Engine::VInt *Engine::VInt::read(SOCKET socket) {
    uint32_t result = 0;
    int position = 0;
    char currentByte;

    while (true) {
        if (recv(socket, &currentByte, 1, 0) == 1) {
            // Accumulate in an unsigned type: shifting a masked byte by 28 would overflow `int` (UB).
            result |= (uint32_t) (currentByte & SEGMENT_BITS) << position;

            if ((currentByte & CONTINUE_BIT) == 0) break;

            position += 7;
            if (position >= 32) {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    return new VInt((int) result);
}

Engine::VInt *Engine::VInt::read(const char *buffer) {
    return read(buffer, MAX_SIZE);
}

Engine::VInt *Engine::VInt::read(const char *buffer, std::size_t length) {
    if (buffer == nullptr) {
        return nullptr;
    }
    uint32_t result = 0;
    int position = 0;
    std::size_t i = 0;

    while (true) {
        if (i >= length) {
            return nullptr; // truncated
        }
        const char currentByte = buffer[i];
        result |= (uint32_t) (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0) break;

        position += 7;
        if (position >= 32) {
            return nullptr;
        }
        i++;
    }

    return new VInt((int) result);
}

int Engine::VInt::write(char *buffer) const {
    int count = 0;
    // Unsigned so that negative values terminate after 5 bytes instead of looping forever
    // (an arithmetic right shift of a negative int never reaches 0).
    uint32_t val = (uint32_t) this->value;

    do {
        unsigned char byte = val & 0x7f;
        val >>= 7;

        if (val != 0)
            byte |= 0x80;  // mark this byte to show that more bytes will follow

        buffer[count] = (char) byte;
        count++;
    } while (val != 0);

    return count;
}

int Engine::VInt::getSize() const {
    int size = 0;
    uint32_t val = (uint32_t) this->value;
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
