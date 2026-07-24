#include "io/Filesystem.h"

char *Engine::Filesystem::readFile(const char *path, int *size) {
    ASSERT("Path is nullptr", path != nullptr);
    std::ifstream stream(path, std::ios::out | std::ios::binary);
    if (!stream) {
        PLOGW << "The requested file [" << path << "] does not exist";
        return nullptr;
    }
    int s = (int) std::filesystem::file_size(path);
    if (!s) {
        PLOGW << "The requested file [" << path << "] does not exist";
        return nullptr;
    }
    char *bin = new char[s];
    stream.read(bin, s);
    stream.close();
    if (size != nullptr) {
        *size = s;
    }
    return bin;
}

bool Engine::Filesystem::writeFile(const char *path, const char *data, unsigned int size) {
    ASSERT("Path is nullptr", path != nullptr);
    ASSERT("Data is nullptr", data != nullptr);
    ASSERT("Size must be >0", size > 0);
    std::ofstream stream(path, std::ios::out | std::ios::binary);
    stream.write(data, size);
    stream.close();
    return stream.good();
}

bool Engine::Filesystem::exists(const char *path) {
    return std::filesystem::exists(path);
}

char *Engine::Filesystem::readString(const char *path) {
    std::ifstream stream(path, std::ios::out | std::ios::binary);
    if (!stream) {
        PLOGW << "The requested string [" << path << "] does not exist";
        return nullptr;
    }
    uintmax_t size = std::filesystem::file_size(path);
    char *bin = new char[size + 1ULL];
    ASSERT("Bin is nullptr", bin != nullptr);
    stream.read(bin, (long long) size);
    stream.close();
    bin[size] = 0;
    return bin;
}

bool Engine::Filesystem::writeString(const char *path, const char *data) {
    return Filesystem::writeFile(path, data, strlen(data));
}

char *Engine::Filesystem::readResourceFile(const char *path, int *size) {
    ASSERT("Path is nullptr", path != nullptr);
    auto myResource = ::FindResource(nullptr, path, RT_RCDATA);
    if (!myResource) {
        PLOGW << "The requested resource file [" << path << "] does not exist";
        return nullptr;
    }
    auto myResourceData = ::LoadResource(nullptr, myResource);
    if (size != nullptr)
        *size = (int) ::SizeofResource(nullptr, myResource);
    return static_cast<char *>(::LockResource(myResourceData));
}

char *Engine::Filesystem::readResourceString(const char *path) {
    ASSERT("Path is nullptr", path != nullptr);
    auto myResource = ::FindResource(nullptr, path, RT_RCDATA);
    if (!myResource) {
        PLOGW << "The requested resource string [" << path << "] does not exist";
        return nullptr;
    }
    auto myResourceData = ::LoadResource(nullptr, myResource);
    auto pMyBinaryData = ::LockResource(myResourceData);
    DWORD size = ::SizeofResource(nullptr, myResource);
    char *str = new char[size + 1];
    ASSERT("Memory for string is nullptr", str != nullptr);
    str[size] = 0;
    memcpy(str, pMyBinaryData, size);
    return str;
}

bool Engine::Filesystem::resourceExists(const char *path) {
    return ::FindResourceA(nullptr, path, RT_RCDATA);
}

unsigned char *Engine::Filesystem::compress(unsigned char *data, unsigned int length, unsigned long *compressedLength) {
    ASSERT("Data is nullptr", data != nullptr);
    ASSERT("Length must be >0", length > 0);
    if (compressedLength != nullptr)
        *compressedLength = 0;
    uLong bound = compressBound(length);
    auto *deflated = new unsigned char[bound];
    uLongf destLen = bound;
    if (compress2(deflated, &destLen, data, length, Z_BEST_COMPRESSION) != Z_OK) {
        PLOGE << "Deflate failed for " << length << " bytes";
        delete[] deflated;
        return nullptr;
    }

    if (compressedLength != nullptr)
        *compressedLength = destLen;

    return deflated;
}

unsigned char *
Engine::Filesystem::decompress(unsigned char *data, unsigned int length, unsigned long *decompressedLength) {
    if (decompressedLength != nullptr)
        *decompressedLength = 0;
    if (data == nullptr || length == 0)
        return nullptr;

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.next_in = data;
    infstream.avail_in = length;
    infstream.total_out = 0;

    if (inflateInit(&infstream) != Z_OK)
        return nullptr;

    size_t capacity = 4096;
    auto *inflated = (unsigned char *) malloc(capacity);
    if (inflated == nullptr) {
        inflateEnd(&infstream);
        return nullptr;
    }

    for (;;) {
        if (infstream.total_out == capacity) {
            capacity *= 2;
            auto *grown = (unsigned char *) realloc(inflated, capacity);
            if (grown == nullptr) {
                free(inflated);
                inflateEnd(&infstream);
                return nullptr;
            }
            inflated = grown;
        }
        infstream.next_out = inflated + infstream.total_out;
        infstream.avail_out = (uInt) (capacity - infstream.total_out);
        int err = inflate(&infstream, Z_NO_FLUSH);
        if (err == Z_STREAM_END) break;
        if (err == Z_OK || (err == Z_BUF_ERROR && infstream.avail_out == 0)) continue;
        PLOGW << "Inflate failed with code " << err << ", input corrupted or truncated";
        free(inflated);
        inflateEnd(&infstream);
        return nullptr;
    }

    if (decompressedLength != nullptr)
        *decompressedLength = infstream.total_out;

    inflateEnd(&infstream);
    return inflated;
}

std::filesystem::path Engine::Filesystem::getWorkingPath() {
    return std::filesystem::current_path();
}
