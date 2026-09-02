#pragma once

#include <filesystem>
#include "plog/Log.h"
#include "Engine.h"

namespace Engine {
    // Ownership: every `char *` / `unsigned char *` returned by this class is allocated with
    // `new[]` and must be released by the caller with `delete[]` (nullptr on failure).
    class Filesystem {
    public:
        static char *readFile(const char *path, int *size = nullptr);

        // Debug builds read plain files; on Windows release builds `path` names an RT_RCDATA
        // resource embedded in the executable (elsewhere it falls back to readFile).
        static char *readResourceFile(const char *path, int *size = nullptr);

        static bool writeFile(const char *path, const char *data, unsigned int size);

        static char *readString(const char *path);

        static char *readResourceString(const char *path);

        static bool writeString(const char *path, const char *data);

        static bool exists(const char *path);

        static bool resourceExists(const char *path);

        static std::filesystem::path getWorkingPath();

        static unsigned char *compress(const unsigned char *data, unsigned int length, unsigned long *compressedLength);

        static unsigned char *decompress(const unsigned char *data, unsigned int length, unsigned long *decompressedLength);
    };
}
