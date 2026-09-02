#include "io/Filesystem.h"

#include <cstring>
#include <fstream>
#include <limits>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#define ZLIB_CONST
#include "zlib.h"

char *Engine::Filesystem::readFile(const char *path, int *size) {
    ASSERT("Path is nullptr", path != nullptr);
    if (size != nullptr) {
        *size = 0;
    }
    std::ifstream stream(path, std::ios::in | std::ios::binary);
    if (!stream) {
        PLOGW << "The requested file [" << path << "] does not exist";
        return nullptr;
    }
    std::error_code ec;
    const uintmax_t fileSize = std::filesystem::file_size(path, ec);
    if (ec || fileSize == 0) {
        PLOGW << "The requested file [" << path << "] is empty or unreadable";
        return nullptr;
    }
    if (fileSize > (uintmax_t) std::numeric_limits<int>::max()) {
        PLOGE << "The requested file [" << path << "] is too large (" << fileSize << " bytes)";
        return nullptr;
    }
    const int s = (int) fileSize;
    char *bin = new char[s];
    if (!stream.read(bin, s)) {
        PLOGE << "The requested file [" << path << "] could not be read completely";
        delete[] bin;
        return nullptr;
    }
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
    if (!stream) {
        PLOGW << "The file [" << path << "] could not be opened for writing";
        return false;
    }
    stream.write(data, size);
    stream.close();
    return stream.good();
}

bool Engine::Filesystem::exists(const char *path) {
    ASSERT("Path is nullptr", path != nullptr);
    std::error_code ec;
    return std::filesystem::exists(path, ec);
}

char *Engine::Filesystem::readString(const char *path) {
    ASSERT("Path is nullptr", path != nullptr);
    std::ifstream stream(path, std::ios::in | std::ios::binary);
    if (!stream) {
        PLOGW << "The requested string [" << path << "] does not exist";
        return nullptr;
    }
    std::error_code ec;
    const uintmax_t size = std::filesystem::file_size(path, ec);
    if (ec) {
        PLOGW << "The requested string [" << path << "] is unreadable";
        return nullptr;
    }
    char *bin = new char[size + 1ULL];
    if (size > 0 && !stream.read(bin, (std::streamsize) size)) {
        PLOGE << "The requested string [" << path << "] could not be read completely";
        delete[] bin;
        return nullptr;
    }
    bin[size] = 0;
    return bin;
}

bool Engine::Filesystem::writeString(const char *path, const char *data) {
    ASSERT("Data is nullptr", data != nullptr);
    return Filesystem::writeFile(path, data, (unsigned int) strlen(data));
}

char *Engine::Filesystem::readResourceFile(const char *path, int *size) {
#ifdef _WIN32
    ASSERT("Path is nullptr", path != nullptr);
    if (size != nullptr)
        *size = 0;
    auto myResource = ::FindResourceA(nullptr, path, RT_RCDATA);
    if (!myResource) {
        PLOGW << "The requested resource file [" << path << "] does not exist";
        return nullptr;
    }
    auto myResourceData = ::LoadResource(nullptr, myResource);
    auto pMyBinaryData = myResourceData ? ::LockResource(myResourceData) : nullptr;
    const DWORD resourceSize = ::SizeofResource(nullptr, myResource);
    if (pMyBinaryData == nullptr || resourceSize == 0) {
        PLOGW << "The requested resource file [" << path << "] could not be loaded";
        return nullptr;
    }
    // Copy out of the module image so the result has the same (delete[]) ownership as readFile.
    char *data = new char[resourceSize];
    memcpy(data, pMyBinaryData, resourceSize);
    if (size != nullptr)
        *size = (int) resourceSize;
    return data;
#else
    return readFile(path, size);
#endif
}

char *Engine::Filesystem::readResourceString(const char *path) {
#ifdef _WIN32
    ASSERT("Path is nullptr", path != nullptr);
    auto myResource = ::FindResourceA(nullptr, path, RT_RCDATA);
    if (!myResource) {
        PLOGW << "The requested resource string [" << path << "] does not exist";
        return nullptr;
    }
    auto myResourceData = ::LoadResource(nullptr, myResource);
    auto pMyBinaryData = myResourceData ? ::LockResource(myResourceData) : nullptr;
    if (pMyBinaryData == nullptr) {
        PLOGW << "The requested resource string [" << path << "] could not be loaded";
        return nullptr;
    }
    DWORD size = ::SizeofResource(nullptr, myResource);
    char *str = new char[size + 1];
    str[size] = 0;
    memcpy(str, pMyBinaryData, size);
    return str;
#else
    return readString(path);
#endif
}

bool Engine::Filesystem::resourceExists(const char *path) {
#ifdef _WIN32
    ASSERT("Path is nullptr", path != nullptr);
    return ::FindResourceA(nullptr, path, RT_RCDATA) != nullptr;
#else
    return exists(path);
#endif
}

unsigned char *Engine::Filesystem::compress(const unsigned char *data, unsigned int length, unsigned long *compressedLength) {
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
Engine::Filesystem::decompress(const unsigned char *data, unsigned int length, unsigned long *decompressedLength) {
    if (decompressedLength != nullptr)
        *decompressedLength = 0;
    if (data == nullptr || length == 0)
        return nullptr;

    z_stream infstream{};
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.next_in = data;
    infstream.avail_in = length;

    if (inflateInit(&infstream) != Z_OK)
        return nullptr;

    // Grow in a vector, then hand out a `new[]` block so the caller frees it like compress()'s result.
    std::vector<unsigned char> inflated(4096);

    for (;;) {
        if (infstream.total_out == inflated.size()) {
            inflated.resize(inflated.size() * 2);
        }
        infstream.next_out = inflated.data() + infstream.total_out;
        infstream.avail_out = (uInt) (inflated.size() - infstream.total_out);
        int err = inflate(&infstream, Z_NO_FLUSH);
        if (err == Z_STREAM_END) break;
        if (err == Z_OK || (err == Z_BUF_ERROR && infstream.avail_out == 0)) continue;
        PLOGW << "Inflate failed with code " << err << ", input corrupted or truncated";
        inflateEnd(&infstream);
        return nullptr;
    }

    const uLong total = infstream.total_out;
    inflateEnd(&infstream);

    auto *result = new unsigned char[total > 0 ? total : 1];
    memcpy(result, inflated.data(), total);
    if (decompressedLength != nullptr)
        *decompressedLength = total;
    return result;
}

std::filesystem::path Engine::Filesystem::getWorkingPath() {
    return std::filesystem::current_path();
}
